/* ***********************static void cleanup_pipeline_resources(t_ast **commands, int **pipes, pid_t *pids,
									   int pipes_created);
static void cleanup_pipeline_memory_only(t_ast **commands, int **pipes, pid_t *pids,
										  int pipes_created);
static void terminate_child_processes(pid_t *pids, int count);************************************************ */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_execution.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 12:44:43 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/06 12:02:32 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"
#include <stdbool.h>

// Déclarations des fonctions statiques
static int wait_for_children(pid_t left_pid, pid_t right_pid);
static int execute_left_pid(int fd[2], pid_t *left_pid, t_ast *node, t_env *env, t_shell *shell);
static int execute_right_pid(int fd[2], pid_t *right_pid, t_ast *node, t_env *env, t_shell *shell);
static int execute_simple_pipe(t_ast *node, t_env *env, t_shell *shell);
static int count_pipeline_commands(t_ast *node);
static void extract_pipeline_commands(t_ast *node, t_ast **commands, int *index);
static bool is_complex_pipeline(t_ast *node);
static void cleanup_pipeline_resources(t_ast **commands, int **pipes, pid_t *pids,
									   int pipes_created);
static void cleanup_pipeline_memory_only(t_ast **commands, int **pipes, pid_t *pids,
										 int pipes_created);
static void cleanup_child_memory_early(t_ast **commands, int **pipes, pid_t *pids,
									   int pipes_created);
static void terminate_child_processes(pid_t *pids, int count);
static int execute_complex_pipeline(t_ast *node, t_env *env, t_shell *shell);
static int allocate_pipeline_resources(int cmd_count, t_ast ***commands,
									   int ***pipes, pid_t **pids);
static int create_all_pipes(int **pipes, int cmd_count);
static int process_all_heredocs(t_ast **commands, int cmd_count, t_shell *shell);
static int create_pipeline_processes(t_ast **commands, int **pipes, pid_t *pids,
									 int cmd_count, t_env *env, t_shell *shell);
static void setup_child_pipes(int **pipes, int cmd_count, int child_index,
							  t_shell *shell);
static int wait_for_all_processes(pid_t *pids, int cmd_count);
static void close_parent_pipes(int **pipes, int cmd_count);
static int setup_pipeline_execution(t_ast *node, t_ast ***commands, int ***pipes,
									pid_t **pids, int *pipes_created, t_shell *shell);
static void setup_left_child_process(int fd[2], t_ast *node, t_env *env, t_shell *shell);
static void setup_right_child_process(int fd[2], t_ast *node, t_env *env, t_shell *shell);
static int process_simple_pipe_heredocs(t_ast *node, t_shell *shell);
static int execute_simple_pipe_processes(int fd[2], t_ast *node, t_env *env, t_shell *shell);
static void initialize_pipeline_pids(pid_t *pids, int cmd_count);
static void execute_pipeline_child(t_ast **commands, int **pipes, pid_t *pids,
								   int cmd_count, int index, t_env *env, t_shell *shell);
static int handle_process_exit_status(int status);
static void setup_child_stdin(int **pipes, int child_index, t_shell *shell);
static void setup_child_stdout(int **pipes, int child_index, int cmd_count, t_shell *shell);
static void close_all_child_pipes(int **pipes, int cmd_count);
static int handle_pipe_execution_error(int fd[2], pid_t left_pid);

/**
 * @brief Attend la fin des deux processus fils d'un pipe.
 *
 * @param left_pid PID du processus gauche
 * @param right_pid PID du processus droit
 * @return Code de retour du processus droit
 */
static int wait_for_children(pid_t left_pid, pid_t right_pid)
{
	int status;

	waitpid(left_pid, &status, 0);
	waitpid(right_pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			write(STDOUT_FILENO, "\n", 1);
		else if (WTERMSIG(status) == SIGQUIT)
			write(STDOUT_FILENO, "Quit (core dumped)\n", 20);
		return (128 + WTERMSIG(status));
	}
	return (1);
}

/**
 * @brief Configure le processus enfant gauche (producteur) d'un pipe.
 *
 * @param fd Le tableau [read, write] du pipe
 * @param node Le nœud contenant la commande à exécuter
 * @param env Variables d'environnement
 * @param shell Structure du shell
 */
static void setup_left_child_process(int fd[2], t_ast *node, t_env *env, t_shell *shell)
{
	int status;

	reset_signals_in_child();
	setup_heredoc_redirection(shell);
	if (dup2(fd[1], STDOUT_FILENO) == -1)
	{
		perror("minishell: dup2");
		cleanup_shell(shell);
		exit(1);
	}
	close(fd[0]);
	close(fd[1]);
	close_all_heredoc_fds(shell);
	status = exec_cmd_no_heredoc(node->left, env, node->left, shell);
	cleanup_shell(shell);
	exit(status);
}

/**
 * @brief Configure le processus enfant droit (consommateur) d'un pipe.
 *
 * @param fd Le tableau [read, write] du pipe
 * @param node Le nœud contenant la commande à exécuter
 * @param env Variables d'environnement
 * @param shell Structure du shell
 */
static void setup_right_child_process(int fd[2], t_ast *node, t_env *env, t_shell *shell)
{
	int status;

	reset_signals_in_child();
	if (dup2(fd[0], STDIN_FILENO) == -1)
	{
		perror("minishell: dup2");
		cleanup_shell(shell);
		exit(1);
	}
	close(fd[0]);
	close(fd[1]);
	close_all_heredoc_fds(shell);
	status = exec_cmd_no_heredoc(node->right, env, node->right, shell);
	cleanup_shell(shell);
	exit(status);
}

/**
 * @brief Traite les heredocs pour un pipe simple.
 *
 * @param node Le nœud racine du pipe
 * @param shell Structure du shell
 * @return 0 en cas de succès, 130 si interruption
 */
static int process_simple_pipe_heredocs(t_ast *node, t_shell *shell)
{
	int heredoc_status;

	heredoc_status = process_heredocs(node->left, shell);
	if (heredoc_status == 130)
		return (130);
	heredoc_status = process_heredocs(node->right, shell);
	if (heredoc_status == 130)
		return (130);
	return (0);
}

/**
 * @brief Exécute les processus d'un pipe simple.
 *
 * @param fd Le pipe créé
 * @param node Le nœud racine du pipe
 * @param env Variables d'environnement
 * @param shell Structure du shell
 * @return Code de retour de la dernière commande
 */
static int execute_simple_pipe_processes(int fd[2], t_ast *node, t_env *env, t_shell *shell)
{
	pid_t left_pid;
	pid_t right_pid;
	int final_status;

	left_pid = -1;
	right_pid = -1;
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	if (execute_left_pid(fd, &left_pid, node, env, shell) != 0)
	{
		close_pipe_fds(fd);
		init_signals();
		return (1);
	}
	if (execute_right_pid(fd, &right_pid, node, env, shell) != 0)
		return (handle_pipe_execution_error(fd, left_pid));
	close_pipe_fds(fd);
	final_status = wait_for_children(left_pid, right_pid);
	init_signals();
	return (final_status);
}

/**
 * @brief Forke et exécute la branche gauche d'un pipe (le producteur).
 *
 * Ce processus écrit dans le pipe. Sa sortie standard (stdout)
 * est redirigée vers l'extrémité d'écriture du pipe.
 *
 * @param fd Le tableau [read, write] du pipe
 * @param left_pid Pointeur vers le PID du processus créé
 * @param node Le nœud de type PIPE contenant la branche gauche
 * @param env Liste chaînée des variables d'environnement
 * @return 0 en cas de succès, 1 sinon
 */
static int execute_left_pid(int fd[2], pid_t *left_pid, t_ast *node, t_env *env, t_shell *shell)
{
	*left_pid = fork();
	if (*left_pid < 0)
	{
		perror("minishell: fork");
		return (1);
	}
	if (*left_pid == 0)
		setup_left_child_process(fd, node, env, shell);
	return (0);
}

/**
 * @brief Forke et exécute la branche droite d'un pipe (le consommateur).
 *
 * Ce processus lit depuis le pipe. Son entrée standard (stdin)
 * est redirigée vers l'extrémité de lecture du pipe.
 *
 * @param fd Le tableau [read, write] du pipe
 * @param right_pid Pointeur vers le PID du processus créé
 * @param node Le nœud de type PIPE contenant la branche droite
 * @param env Liste chaînée des variables d'environnement
 * @return 0 en cas de succès, 1 sinon
 */
static int execute_right_pid(int fd[2], pid_t *right_pid, t_ast *node, t_env *env, t_shell *shell)
{
	*right_pid = fork();
	if (*right_pid < 0)
	{
		perror("minishell: fork");
		return (1);
	}
	if (*right_pid == 0)
		setup_right_child_process(fd, node, env, shell);
	return (0);
}

/**
 * @brief Exécute un nœud de type PIPE de l'AST.
 *
 * Détecte si c'est un pipeline simple (2 commandes) ou complexe (plus de 2),
 * puis utilise la logique appropriée pour l'exécution.
 *
 * @param node Le nœud de type PIPE
 * @param env Liste chaînée des variables d'environnement
 * @param shell Structure principale du shell
 * @return Code de retour de la dernière commande du pipeline
 */
int execute_pipe_node(t_ast *node, t_env *env, t_shell *shell)
{
	// Si c'est un pipeline complexe (plus de 2 commandes)
	if (is_complex_pipeline(node))
		return (execute_complex_pipeline(node, env, shell));

	// Sinon, utiliser l'ancienne logique pour les pipes simples
	return (execute_simple_pipe(node, env, shell));
}

/**
 * @brief Exécute un pipe simple (2 commandes seulement).
 *
 * Traite d'abord tous les heredocs au niveau parent,
 * puis crée un pipe, fork deux processus (gauche et droit),
 * connecte leur stdin/stdout via le pipe, puis attend leur fin.
 *
 * @param node Le nœud de type PIPE
 * @param env Liste chaînée des variables d'environnement
 * @param shell Structure principale du shell
 * @return Code de retour du processus droit (dernière commande du pipe)
 */
static int execute_simple_pipe(t_ast *node, t_env *env, t_shell *shell)
{
	int fd[2];
	int final_status;
	int heredoc_result;

	final_status = 1;
	heredoc_result = process_simple_pipe_heredocs(node, shell);
	if (heredoc_result != 0)
		return (heredoc_result);
	if (pipe(fd) == -1)
	{
		perror("minishell: pipe");
		return (1);
	}
	final_status = execute_simple_pipe_processes(fd, node, env, shell);
	close_all_heredoc_fds(shell);
	free_all_heredoc_fds(shell);
	return (final_status);
}

/**
 * @brief Configure l'entrée standard pour un processus enfant.
 *
 * @param pipes Tableau des pipes
 * @param child_index Index du processus enfant
 * @param shell Structure du shell
 */
static void setup_child_stdin(int **pipes, int child_index, t_shell *shell)
{
	if (child_index > 0)
	{
		if (dup2(pipes[child_index - 1][0], STDIN_FILENO) == -1)
		{
			perror("minishell: dup2 stdin");
			cleanup_shell(shell);
			exit(1);
		}
	}
}

/**
 * @brief Configure la sortie standard pour un processus enfant.
 *
 * @param pipes Tableau des pipes
 * @param child_index Index du processus enfant
 * @param cmd_count Nombre total de commandes
 * @param shell Structure du shell
 */
static void setup_child_stdout(int **pipes, int child_index, int cmd_count, t_shell *shell)
{
	if (child_index < cmd_count - 1)
	{
		if (dup2(pipes[child_index][1], STDOUT_FILENO) == -1)
		{
			perror("minishell: dup2 stdout");
			cleanup_shell(shell);
			exit(1);
		}
	}
}

/**
 * @brief Ferme tous les pipes dans un processus enfant.
 *
 * @param pipes Tableau des pipes
 * @param cmd_count Nombre de commandes
 */
static void close_all_child_pipes(int **pipes, int cmd_count)
{
	int j;

	j = 0;
	while (j < cmd_count - 1)
	{
		close(pipes[j][0]);
		close(pipes[j][1]);
		j++;
	}
}

/**
 * @brief Gère les erreurs lors de l'exécution des processus du pipe.
 *
 * @param fd Le pipe à fermer
 * @param left_pid PID du processus gauche à terminer
 * @return Code d'erreur (1)
 */
static int handle_pipe_execution_error(int fd[2], pid_t left_pid)
{
	close_pipe_fds(fd);
	if (left_pid > 0)
	{
		kill(left_pid, SIGTERM);
		waitpid(left_pid, NULL, 0);
	}
	init_signals();
	return (1);
}

/**
 * @brief Initialise tous les PIDs du pipeline à -1.
 *
 * @param pids Tableau des PIDs
 * @param cmd_count Nombre de commandes
 */
static void initialize_pipeline_pids(pid_t *pids, int cmd_count)
{
	int i;

	i = 0;
	while (i < cmd_count)
	{
		pids[i] = -1;
		i++;
	}
}

/**
 * @brief Exécute la logique d'un processus enfant dans le pipeline.
 *
 * @param commands Tableau des commandes
 * @param pipes Tableau des pipes
 * @param pids Tableau des PIDs
 * @param cmd_count Nombre de commandes
 * @param index Index du processus courant
 * @param env Variables d'environnement
 * @param shell Structure du shell
 */
static void execute_pipeline_child(t_ast **commands, int **pipes, pid_t *pids,
								   int cmd_count, int index, t_env *env, t_shell *shell)
{
	t_ast *current_cmd;
	int status;

	reset_signals_in_child();
	setup_heredoc_redirection(shell);
	current_cmd = commands[index];
	setup_child_pipes(pipes, cmd_count, index, shell);
	close_all_heredoc_fds(shell);
	cleanup_child_memory_early(commands, pipes, pids, cmd_count - 1);
	status = exec_cmd_no_heredoc(current_cmd, env, current_cmd, shell);
	cleanup_shell(shell);
	exit(status);
}

/**
 * @brief Gère le statut de sortie d'un processus.
 *
 * @param status Statut de sortie du processus
 * @return Code de retour approprié
 */
static int handle_process_exit_status(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			write(STDOUT_FILENO, "\n", 1);
		else if (WTERMSIG(status) == SIGQUIT)
			write(STDOUT_FILENO, "Quit (core dumped)\n", 20);
		return (128 + WTERMSIG(status));
	}
	return (1);
}

/**
 * @brief Compte le nombre de commandes dans un pipeline.
 *
 * @param node Le nœud racine du pipeline
 * @return Le nombre de commandes dans le pipeline
 */
static int count_pipeline_commands(t_ast *node)
{
	if (!node)
		return (0);
	if (node->type == CMD)
		return (1);
	if (node->type == PIPE)
		return (count_pipeline_commands(node->left) + count_pipeline_commands(node->right));
	// Pour les redirections, vérifier s'il y a une commande sous-jacente
	if (find_cmd_node(node))
		return (1);
	return (0);
}

/**
 * @brief Extrait toutes les commandes d'un pipeline dans un tableau.
 *
 * @param node Le nœud racine du pipeline
 * @param commands Tableau pour stocker les commandes
 * @param index Pointeur vers l'index courant dans le tableau
 */
static void extract_pipeline_commands(t_ast *node, t_ast **commands, int *index)
{
	if (!node)
		return;

	if (node->type == PIPE)
	{
		// Pour les pipes, toujours traiter gauche puis droite pour maintenir l'ordre
		extract_pipeline_commands(node->left, commands, index);
		extract_pipeline_commands(node->right, commands, index);
		return;
	}

	if (node->type == CMD)
	{
		commands[*index] = node;
		(*index)++;
		return;
	}

	// Pour les redirections, chercher la commande sous-jacente
	t_ast *cmd_node = find_cmd_node(node);
	if (cmd_node)
	{
		commands[*index] = node; // Garder le nœud avec redirections
		(*index)++;
	}
}

/**
 * @brief Vérifie si un nœud fait partie d'un pipeline complexe (plus de 2 commandes).
 *
 * @param node Le nœud à vérifier
 * @return true si c'est un pipeline complexe, false sinon
 */
static bool is_complex_pipeline(t_ast *node)
{
	return (count_pipeline_commands(node) > 2);
}

/**
 * @brief Nettoie toutes les ressources allouées pour un pipeline.
 *
 * @param commands Tableau des commandes
 * @param pipes Tableau des pipes
 * @param pids Tableau des PIDs
 * @param pipes_created Nombre de pipes créés avec succès
 */
static void cleanup_pipeline_resources(t_ast **commands, int **pipes, pid_t *pids,
									   int pipes_created)
{
	int i;

	i = 0;
	// Nettoyage des pipes
	if (pipes)
	{
		while (i < pipes_created)
		{
			if (pipes[i])
			{
				close(pipes[i][0]);
				close(pipes[i][1]);
				free(pipes[i]);
			}
			i++;
		}
		free(pipes);
	}

	// Nettoyage des autres allocations
	if (commands)
		free(commands);
	if (pids)
		free(pids);
}

/**
 * @brief Termine tous les processus enfants créés en cas d'erreur.
 *
 * @param pids Tableau des PIDs
 * @param count Nombre de processus à terminer
 */
static void terminate_child_processes(pid_t *pids, int count)
{
	int i;

	i = 0;
	while (i < count)
	{
		if (pids[i] > 0)
			kill(pids[i], SIGTERM);
		i++;
	}
	// Attendre que les processus se terminent
	i = 0;
	while (i < count)
	{
		if (pids[i] > 0)
			waitpid(pids[i], NULL, 0);
		i++;
	}
}

/**
 * @brief Alloue toutes les ressources nécessaires pour un pipeline complexe.
 *
 * @param cmd_count Nombre de commandes dans le pipeline
 * @param commands Pointeur vers le tableau des commandes
 * @param pipes Pointeur vers le tableau des pipes
 * @param pids Pointeur vers le tableau des PIDs
 * @return 0 en cas de succès, 1 sinon
 */
static int allocate_pipeline_resources(int cmd_count, t_ast ***commands,
									   int ***pipes, pid_t **pids)
{
	*commands = malloc(cmd_count * sizeof(t_ast *));
	if (!*commands)
		return (1);
	*pipes = malloc((cmd_count - 1) * sizeof(int *));
	if (!*pipes)
	{
		free(*commands);
		return (1);
	}
	*pids = malloc(cmd_count * sizeof(pid_t));
	if (!*pids)
	{
		free(*commands);
		free(*pipes);
		return (1);
	}
	return (0);
}

/**
 * @brief Crée tous les pipes nécessaires pour le pipeline.
 *
 * @param pipes Tableau des pipes
 * @param cmd_count Nombre de commandes
 * @return Nombre de pipes créés avec succès, -1 en cas d'erreur
 */
static int create_all_pipes(int **pipes, int cmd_count)
{
	int i;

	i = 0;
	while (i < cmd_count - 1)
	{
		pipes[i] = NULL;
		i++;
	}
	i = 0;
	while (i < cmd_count - 1)
	{
		pipes[i] = malloc(2 * sizeof(int));
		if (!pipes[i])
			return (i);
		if (pipe(pipes[i]) == -1)
		{
			perror("minishell: pipe");
			free(pipes[i]);
			pipes[i] = NULL;
			return (i);
		}
		i++;
	}
	return (cmd_count - 1);
}

/**
 * @brief Traite tous les heredocs pour toutes les commandes du pipeline.
 *
 * @param commands Tableau des commandes
 * @param cmd_count Nombre de commandes
 * @param shell Structure du shell
 * @return 0 en cas de succès, 130 si interruption
 */
static int process_all_heredocs(t_ast **commands, int cmd_count, t_shell *shell)
{
	int i;
	int heredoc_status;

	i = 0;
	while (i < cmd_count)
	{
		heredoc_status = process_heredocs(commands[i], shell);
		if (heredoc_status == 130)
			return (130);
		i++;
	}
	return (0);
}

/**
 * @brief Configure les pipes pour un processus enfant spécifique.
 *
 * @param pipes Tableau des pipes
 * @param cmd_count Nombre total de commandes
 * @param child_index Index du processus enfant
 * @param shell Structure du shell (pour cleanup en cas d'erreur)
 */
static void setup_child_pipes(int **pipes, int cmd_count, int child_index,
							  t_shell *shell)
{
	setup_child_stdin(pipes, child_index, shell);
	setup_child_stdout(pipes, child_index, cmd_count, shell);
	close_all_child_pipes(pipes, cmd_count);
}

/**
 * @brief Attend tous les processus enfants et récupère le code de retour.
 *
 * @param pids Tableau des PIDs
 * @param cmd_count Nombre de processus
 * @return Code de retour de la dernière commande
 */
static int wait_for_all_processes(pid_t *pids, int cmd_count)
{
	int i;
	int status;
	int final_status;

	i = 0;
	final_status = 0;
	while (i < cmd_count)
	{
		waitpid(pids[i], &status, 0);
		if (i == cmd_count - 1)
			final_status = handle_process_exit_status(status);
		i++;
	}
	return (final_status);
}

/**
 * @brief Crée tous les processus enfants pour le pipeline.
 *
 * @param commands Tableau des commandes
 * @param pipes Tableau des pipes
 * @param pids Tableau des PIDs
 * @param cmd_count Nombre de commandes
 * @param env Variables d'environnement
 * @param shell Structure du shell
 * @return 0 en cas de succès, 1 sinon
 */
static int create_pipeline_processes(t_ast **commands, int **pipes, pid_t *pids,
									 int cmd_count, t_env *env, t_shell *shell)
{
	int i;

	initialize_pipeline_pids(pids, cmd_count);
	i = 0;
	while (i < cmd_count)
	{
		pids[i] = fork();
		if (pids[i] < 0)
		{
			perror("minishell: fork");
			terminate_child_processes(pids, i);
			return (1);
		}
		if (pids[i] == 0)
			execute_pipeline_child(commands, pipes, pids, cmd_count, i, env, shell);
		i++;
	}
	return (0);
}

/**
 * @brief Ferme tous les pipes dans le processus parent.
 *
 * @param pipes Tableau des pipes
 * @param cmd_count Nombre de commandes
 */
static void close_parent_pipes(int **pipes, int cmd_count)
{
	int i;

	i = 0;
	while (i < cmd_count - 1)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}

/**
 * @brief Configure et prépare tous les éléments nécessaires pour l'exécution du pipeline.
 *
 * @param node Le nœud racine du pipeline
 * @param commands Pointeur vers le tableau des commandes
 * @param pipes Pointeur vers le tableau des pipes
 * @param pids Pointeur vers le tableau des PIDs
 * @param pipes_created Pointeur vers le nombre de pipes créés
 * @param shell Structure du shell
 * @return 0 en cas de succès, code d'erreur sinon
 */
static int setup_pipeline_execution(t_ast *node, t_ast ***commands, int ***pipes,
									pid_t **pids, int *pipes_created, t_shell *shell)
{
	int cmd_count;
	int index;

	cmd_count = count_pipeline_commands(node);
	index = 0;
	if (cmd_count <= 0)
		return (1);
	if (allocate_pipeline_resources(cmd_count, commands, pipes, pids) != 0)
		return (1);
	*pipes_created = create_all_pipes(*pipes, cmd_count);
	if (*pipes_created != cmd_count - 1)
	{
		cleanup_pipeline_resources(*commands, *pipes, *pids, *pipes_created);
		return (1);
	}
	extract_pipeline_commands(node, *commands, &index);
	if (process_all_heredocs(*commands, cmd_count, shell) == 130)
	{
		cleanup_pipeline_resources(*commands, *pipes, *pids, *pipes_created);
		return (130);
	}
	return (0);
}
/**
 * @brief Exécute un pipeline complet avec plusieurs commandes.
 *
 * Orchestrates the execution of a complex pipeline by allocating resources,
 * creating pipes, processing heredocs, and coordinating child processes.
 *
 * @param node Le nœud racine du pipeline
 * @param env Liste des variables d'environnement
 * @param shell Structure du shell
 * @return Code de retour de la dernière commande du pipeline
 */
static int execute_complex_pipeline(t_ast *node, t_env *env, t_shell *shell)
{
	t_ast **commands;
	int **pipes;
	pid_t *pids;
	int pipes_created;
	int final_status;
	int setup_result;

	commands = NULL;
	pipes = NULL;
	pids = NULL;
	setup_result = setup_pipeline_execution(node, &commands, &pipes, &pids,
											&pipes_created, shell);
	if (setup_result != 0)
		return (setup_result);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	if (create_pipeline_processes(commands, pipes, pids,
								  count_pipeline_commands(node), env, shell) != 0)
	{
		cleanup_pipeline_resources(commands, pipes, pids, pipes_created);
		init_signals();
		return (1);
	}
	close_parent_pipes(pipes, count_pipeline_commands(node));
	final_status = wait_for_all_processes(pids, count_pipeline_commands(node));
	init_signals();
	close_all_heredoc_fds(shell);
	free_all_heredoc_fds(shell);
	cleanup_pipeline_memory_only(commands, pipes, pids, pipes_created);
	return (final_status);
}

/**
 * @brief Nettoie la mémoire allouée pour un pipeline sans fermer les pipes.
 *
 * Utilisé à la fin de l'exécution du pipeline quand les pipes ont déjà été fermés.
 *
 * @param commands Tableau des commandes
 * @param pipes Tableau des pipes
 * @param pids Tableau des PIDs
 * @param pipes_created Nombre de pipes créés avec succès
 */
static void cleanup_pipeline_memory_only(t_ast **commands, int **pipes, pid_t *pids,
										 int pipes_created)
{
	int i;

	i = 0;
	// Libérer la mémoire des pipes sans les fermer (déjà fermés)
	if (pipes)
	{
		while (i < pipes_created)
		{
			if (pipes[i])
				free(pipes[i]);
			i++;
		}
		free(pipes);
	}

	// Nettoyage des autres allocations
	if (commands)
		free(commands);
	if (pids)
		free(pids);
}

/**
 * @brief Libère la mémoire des pipes et tableaux dans le processus enfant immédiatement après fork.
 *
 * Version optimisée qui libère uniquement la mémoire sans fermer les pipes
 * (car ils sont encore nécessaires pour les redirections).
 * Utilisée dans les processus enfants pour éviter les fuites "still reachable".
 *
 * @param commands Tableau des commandes
 * @param pipes Tableau des pipes (libéré mais les fd restent ouverts)
 * @param pids Tableau des PIDs
 * @param pipes_created Nombre de pipes créés
 */
static void cleanup_child_memory_early(t_ast **commands, int **pipes, pid_t *pids,
									   int pipes_created)
{
	int i;

	i = 0;
	// Libérer seulement la mémoire des structures de données,
	// PAS les fd des pipes qui sont encore utilisés
	if (pipes)
	{
		while (i < pipes_created)
		{
			if (pipes[i])
				free(pipes[i]); // Libère le malloc de int[2] mais garde les fd ouverts
			i++;
		}
		free(pipes);
	}

	if (commands)
		free(commands);
	if (pids)
		free(pids);
}
