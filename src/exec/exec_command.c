/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 08:32:17 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/06 12:29:37 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"
#include <errno.h>

/**
 * @brief Cherche récursivement le premier noeud de type CMD.
 *
 * @param node Nœud racine du sous-arbre AST.
 * @return Pointeur vers le noeud de type CMD, ou NULL si non trouvé.
 */
t_ast *find_cmd_node(t_ast *node)
{
	t_ast *found;

	// Si le noeud est nul, on ne trouve rien
	if (!node)
		return (NULL);
	// Si le noeud courant est un CMD, on le retourne
	if (node->type == CMD)
		return (node);
	// Recherche récursive dans la branche gauche
	found = find_cmd_node(node->left);
	if (found)
		return (found);
	// Sinon, recherche dans la branche droite
	return (find_cmd_node(node->right));
}

/**
 * @brief Résout le chemin de la commande et effectue les vérifications.
 *
 * @param argv Arguments de la commande
 * @param env Variables d'environnement
 * @param shell Structure du shell
 * @return Chemin résolu de la commande
 */
static char *resolve_and_check_command(char **argv, t_env *env, t_shell *shell)
{
	char *path;

	path = resolve_command_path(argv[0], env);
	if (!path)
	{
		print_command_not_found_error(argv[0]);
		cleanup_shell(shell);
		exit(127);
	}
	if (is_directory(path))
	{
		free(path);
		cleanup_shell(shell);
		exit(126);
	}
	return (path);
}

/**
 * @brief Exécute la commande dans le processus enfant.
 *
 * @param argv Arguments de la commande
 * @param env Variables d'environnement
 * @param ast Nœud AST pour les redirections
 * @param shell Structure du shell
 */
static void run_child_process(char **argv, t_env *env,
							  t_ast *ast, t_shell *shell)
{
	char *path;
	char **envp;

	reset_signals_in_child();
	setup_heredoc_redirection(shell);
	if (setup_redirections(ast) != 0)
	{
		cleanup_shell(shell);
		exit(1);
	}
	path = resolve_and_check_command(argv, env, shell);
	envp = env_to_char_array(env);
	if (!envp)
	{
		perror("minishell: env malloc");
		cleanup_shell(shell);
		exit(1);
	}
	if (execve(path, argv, envp) == -1)
	{
		if (errno != ENOEXEC)
			perror("minishell: execve");
		free_child_exec(path, envp, shell);
		exit(127);
	}
}

/**
 * @brief Exécute un processus enfant sans configurer les redirections heredoc.
 *
 * Version utilisée dans les pipes où les redirections heredoc
 * ont déjà été configurées par le processus parent du pipe.
 */
static void run_child_process_no_heredoc(char **argv, t_env *env,
										 t_ast *ast, t_shell *shell)
{
	char *path;
	char **envp;

	reset_signals_in_child();
	// PAS de setup_heredoc_redirection ici !
	if (setup_redirections(ast) != 0)
	{
		cleanup_shell(shell);
		exit(1);
	}
	path = resolve_and_check_command(argv, env, shell);
	envp = env_to_char_array(env);
	if (!path || !envp)
	{
		free_child_exec(path, envp, shell);
		exit(127);
	}
	if (execve(path, argv, envp) == -1)
	{
		if (errno != ENOEXEC)
			perror("minishell: execve");
		free_child_exec(path, envp, shell);
		exit(127);
	}
}

/**
 * @brief Gère le code retour du processus enfant après un waitpid.
 *
 * @param status Statut renvoyé par waitpid
 * @return Code de sortie du shell pour cette commande
 */
static int handle_child_status(int status)
{
	// Si l'enfant a été tué par un signal
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			write(STDOUT_FILENO, "\n", 1);
		else if (WTERMSIG(status) == SIGQUIT)
			write(STDOUT_FILENO, "Quit (core dumped)\n", 20);
		return (128 + WTERMSIG(status));
	}
	// Si l'enfant s'est terminé normalement
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	// Par défaut
	return (1);
}

/**
 * @brief Exécute un processus enfant et attend sa terminaison.
 *
 * @param cmd_node Nœud de commande
 * @param env Variables d'environnement
 * @param ast_root Racine de l'AST
 * @param shell Structure du shell
 * @return Code de retour du processus enfant
 */
int execute_fork_process(t_ast *cmd_node, t_env *env,
						 t_ast *ast_root, t_shell *shell)
{
	pid_t pid;
	int status;

	pid = fork();
	if (pid < 0)
	{
		perror("minishell: fork");
		return (1);
	}
	if (pid == 0)
		run_child_process(cmd_node->args, env, ast_root, shell);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	waitpid(pid, &status, 0);
	init_signals();
	// Fermer tous les fd heredoc ouverts après l'exécution du process
	close_all_heredoc_fds(shell);
	free_all_heredoc_fds(shell);
	return (handle_child_status(status));
}

/**
 * @brief Exécute un processus enfant sans traiter les heredocs (pour les pipes).
 *
 * @param cmd_node Nœud de commande
 * @param env Variables d'environnement
 * @param ast_root Racine de l'AST
 * @param shell Structure du shell
 * @return Code de retour du processus enfant
 */
int execute_fork_process_no_heredoc(t_ast *cmd_node, t_env *env,
									t_ast *ast_root, t_shell *shell)
{
	pid_t pid;
	int status;

	pid = fork();
	if (pid < 0)
	{
		perror("minishell: fork");
		return (1);
	}
	if (pid == 0)
		run_child_process_no_heredoc(cmd_node->args, env, ast_root, shell);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	waitpid(pid, &status, 0);
	init_signals();
	// PAS de fermeture des heredoc fd ici car ils sont gérés par le parent du pipe
	return (handle_child_status(status));
}
