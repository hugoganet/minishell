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
	int status;

	*left_pid = fork();
	if (*left_pid < 0)
	{
		perror("minishell: fork");
		return (1);
	}
	if (*left_pid == 0)
	{
		// Processus enfant - restaurer les signaux par défaut
		reset_signals_in_child();
		// Configurer les redirections heredoc AVANT de fermer les fd
		setup_heredoc_redirection(shell);
		// Redirige stdout vers l'extrémité d'écriture du pipe
		if (dup2(fd[1], STDOUT_FILENO) == -1)
		{
			perror("minishell: dup2");
			cleanup_shell(shell);
			exit(1);
		}
		close(fd[0]);
		close(fd[1]);
		// Fermer tous les fd heredoc hérités du parent (après utilisation)
		close_all_heredoc_fds(shell);
		status = exec_cmd_no_heredoc(node->left, env, node->left, shell);
		cleanup_shell(shell);
		exit(status);
	}
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
	int status;

	*right_pid = fork();
	if (*right_pid < 0)
	{
		perror("minishell: fork");
		return (1);
	}
	if (*right_pid == 0)
	{
		// Processus enfant - restaurer les signaux par défaut
		reset_signals_in_child();
		// Redirige stdin depuis l'extrémité de lecture du pipe
		if (dup2(fd[0], STDIN_FILENO) == -1)
		{
			perror("minishell: dup2");
			cleanup_shell(shell);
			exit(1);
		}
		close(fd[0]);
		close(fd[1]);
		// Fermer tous les fd heredoc hérités du parent
		close_all_heredoc_fds(shell);
		status = exec_cmd_no_heredoc(node->right, env, node->right, shell);
		cleanup_shell(shell);
		exit(status);
	}
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
	pid_t left_pid = -1, right_pid = -1;
	int heredoc_status;
	int final_status = 1;

	// Traiter tous les heredocs dans le processus parent AVANT de créer les pipes
	heredoc_status = process_heredocs(node->left, shell);
	if (heredoc_status == 130)
		return (130);
	heredoc_status = process_heredocs(node->right, shell);
	if (heredoc_status == 130)
		return (130);

	if (pipe(fd) == -1)
	{
		perror("minishell: pipe");
		return (1);
	}

	// Ignorer les signaux pendant l'exécution du pipe
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);

	if (execute_left_pid(fd, &left_pid, node, env, shell) != 0)
	{
		close_pipe_fds(fd);
		init_signals();
		return (1);
	}
	if (execute_right_pid(fd, &right_pid, node, env, shell) != 0)
	{
		close_pipe_fds(fd);
		// Terminer le processus gauche si le droit a échoué
		if (left_pid > 0)
		{
			kill(left_pid, SIGTERM);
			waitpid(left_pid, NULL, 0);
		}
		init_signals();
		return (1);
	}

	close_pipe_fds(fd);
	final_status = wait_for_children(left_pid, right_pid);

	// Restaurer les signaux
	init_signals();

	// Fermer et libérer tous les fd heredoc après l'exécution du pipe
	close_all_heredoc_fds(shell);
	free_all_heredoc_fds(shell);

	return (final_status);
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

	// Nettoyage des pipes
	if (pipes)
	{
		for (i = 0; i < pipes_created; i++)
		{
			if (pipes[i])
			{
				close(pipes[i][0]);
				close(pipes[i][1]);
				free(pipes[i]);
			}
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

	for (i = 0; i < count; i++)
	{
		if (pids[i] > 0)
			kill(pids[i], SIGTERM);
	}
	// Attendre que les processus se terminent
	for (i = 0; i < count; i++)
	{
		if (pids[i] > 0)
			waitpid(pids[i], NULL, 0);
	}
}

/**
 * @brief Exécute un pipeline complet avec plusieurs commandes.
 *
 * Crée tous les pipes nécessaires, fork tous les processus,
 * et connecte correctement les stdin/stdout entre les commandes.
 *
 * @param node Le nœud racine du pipeline
 * @param env Liste des variables d'environnement
 * @param shell Structure du shell
 * @return Code de retour de la dernière commande du pipeline
 */
static int execute_complex_pipeline(t_ast *node, t_env *env, t_shell *shell)
{
	int cmd_count = count_pipeline_commands(node);
	t_ast **commands = NULL;
	int **pipes = NULL;
	pid_t *pids = NULL;
	int i, j, status, final_status = 0;
	int index = 0;
	int pipes_created = 0;

	// Vérification du nombre de commandes
	if (cmd_count <= 0)
		return (1);

	// Allocation des tableaux avec vérification individuelle
	commands = malloc(cmd_count * sizeof(t_ast *));
	if (!commands)
		return (1);

	pipes = malloc((cmd_count - 1) * sizeof(int *));
	if (!pipes)
	{
		free(commands);
		return (1);
	}

	pids = malloc(cmd_count * sizeof(pid_t));
	if (!pids)
	{
		free(commands);
		free(pipes);
		return (1);
	}

	// Initialisation du tableau de pipes
	for (i = 0; i < cmd_count - 1; i++)
		pipes[i] = NULL;

	// Initialisation du tableau de PIDs
	for (i = 0; i < cmd_count; i++)
		pids[i] = -1;

	// Allocation et création des pipes individuels
	for (i = 0; i < cmd_count - 1; i++)
	{
		pipes[i] = malloc(2 * sizeof(int));
		if (!pipes[i])
		{
			cleanup_pipeline_resources(commands, pipes, pids, i);
			return (1);
		}
		if (pipe(pipes[i]) == -1)
		{
			perror("minishell: pipe");
			free(pipes[i]); // Libérer le malloc qui a réussi mais le pipe a échoué
			pipes[i] = NULL;
			cleanup_pipeline_resources(commands, pipes, pids, i);
			return (1);
		}
		// Seulement incrémenter après succès complet de malloc + pipe
		pipes_created = i + 1;
	}

	// Extraction des commandes
	extract_pipeline_commands(node, commands, &index);

	// Traitement des heredocs avant de créer les processus
	for (i = 0; i < cmd_count; i++)
	{
		int heredoc_status = process_heredocs(commands[i], shell);
		if (heredoc_status == 130)
		{
			cleanup_pipeline_resources(commands, pipes, pids, pipes_created);
			return (130);
		}
	}

	// Ignorer les signaux dans le processus parent pendant l'exécution du pipeline
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);

	// Création des processus pour chaque commande
	for (i = 0; i < cmd_count; i++)
	{
		pids[i] = fork();
		if (pids[i] < 0)
		{
			perror("minishell: fork");
			// Terminer les processus déjà créés
			terminate_child_processes(pids, i);
			cleanup_pipeline_resources(commands, pipes, pids, pipes_created);
			init_signals(); // Restaurer les signaux
			return (1);
		}

		if (pids[i] == 0)
		{
			// Processus enfant - restaurer les signaux par défaut
			reset_signals_in_child();
			setup_heredoc_redirection(shell);

			// IMPORTANT: Sauvegarder la commande courante AVANT de libérer la mémoire
			t_ast *current_cmd = commands[i];

			// Configuration des pipes pour ce processus
			if (i > 0) // Pas la première commande : lire depuis le pipe précédent
			{
				if (dup2(pipes[i - 1][0], STDIN_FILENO) == -1)
				{
					perror("minishell: dup2 stdin");
					cleanup_shell(shell);
					exit(1);
				}
			}
			if (i < cmd_count - 1) // Pas la dernière commande : écrire vers le pipe suivant
			{
				if (dup2(pipes[i][1], STDOUT_FILENO) == -1)
				{
					perror("minishell: dup2 stdout");
					cleanup_shell(shell);
					exit(1);
				}
			}

			// Fermeture de tous les pipes dans le processus enfant
			for (j = 0; j < cmd_count - 1; j++)
			{
				close(pipes[j][0]);
				close(pipes[j][1]);
			}

			close_all_heredoc_fds(shell);

			// CRUCIAL: Libérer la mémoire héritée du parent IMMÉDIATEMENT après config des pipes
			cleanup_child_memory_early(commands, pipes, pids, pipes_created);

			// Utiliser la commande sauvegardée (current_cmd) au lieu de commands[i]
			status = exec_cmd_no_heredoc(current_cmd, env, current_cmd, shell);
			cleanup_shell(shell);
			exit(status);
		}
	}

	// Fermeture des pipes dans le processus parent
	for (i = 0; i < cmd_count - 1; i++)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
	}

	// Attendre tous les processus enfants
	for (i = 0; i < cmd_count; i++)
	{
		waitpid(pids[i], &status, 0);
		if (i == cmd_count - 1) // Récupérer le code de retour de la dernière commande
		{
			if (WIFEXITED(status))
				final_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
			{
				if (WTERMSIG(status) == SIGINT)
					write(STDOUT_FILENO, "\n", 1);
				else if (WTERMSIG(status) == SIGQUIT)
					write(STDOUT_FILENO, "Quit (core dumped)\n", 20);
				final_status = 128 + WTERMSIG(status);
			}
			else
				final_status = 1;
		}
	}

	// Restaurer les signaux
	init_signals();

	// Fermer et libérer tous les fd heredoc après l'exécution du pipeline
	close_all_heredoc_fds(shell);
	free_all_heredoc_fds(shell);

	// Nettoyage final (les pipes sont déjà fermés)
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

	// Libérer la mémoire des pipes sans les fermer (déjà fermés)
	if (pipes)
	{
		for (i = 0; i < pipes_created; i++)
		{
			if (pipes[i])
				free(pipes[i]);
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

	// Libérer seulement la mémoire des structures de données,
	// PAS les fd des pipes qui sont encore utilisés
	if (pipes)
	{
		for (i = 0; i < pipes_created; i++)
		{
			if (pipes[i])
				free(pipes[i]); // Libère le malloc de int[2] mais garde les fd ouverts
		}
		free(pipes);
	}

	if (commands)
		free(commands);
	if (pids)
		free(pids);
}
