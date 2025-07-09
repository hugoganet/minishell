/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_fork.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 00:00:00 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/09 18:52:38 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"
#include <errno.h>

/**
 * @brief Exécute le code d'un processus enfant sans gestion d'heredoc.
 *
 * Configure les signaux, applique les redirections, résout le chemin de la commande,
 * prépare l'environnement, puis exécute la commande avec execve.
 * Nettoie les ressources et gère les erreurs d'exécution.
 *
 * @param argv Tableau d'arguments de la commande
 * @param env Liste chaînée des variables d'environnement
 * @param ast AST de la commande (pour les redirections)
 * @param shell Structure principale du shell
 */
static void run_child_process_no_heredoc(char **argv, t_env *env,
										 t_ast *ast, t_shell *shell)
{
	char *path;
	char **envp;

	reset_signals_in_child();
	if (setup_redirections(ast) != 0)
	{
		cleanup_shell(shell);
		exit(1);
	}
	path = resolve_command_path(argv[0], env);
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
 * @brief Analyse le statut de sortie d'un processus enfant.
 *
 * Gère les cas de terminaison par signal (SIGINT, SIGQUIT) et de sortie normale.
 * Affiche les messages appropriés et retourne le code de sortie à propager au shell.
 *
 * @param status Statut retourné par waitpid
 * @return Code de sortie à utiliser pour le shell
 */
static int handle_child_status(int status)
{
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			write(STDOUT_FILENO, "\n", 1);
		else if (WTERMSIG(status) == SIGQUIT)
			write(STDOUT_FILENO, "Quit (core dumped)\n", 20);
		return (128 + WTERMSIG(status));
	}
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

/**
 * @brief Exécute une commande dans un processus enfant avec gestion des heredocs.
 *
 * Fork un nouveau processus, exécute la commande (avec redirections et heredocs),
 * gère les signaux, attend la fin du processus et retourne le code de sortie.
 *
 * @param cmd_node Nœud AST de la commande à exécuter
 * @param env Liste chaînée des variables d'environnement
 * @param ast_root AST racine (pour les redirections/heredocs)
 * @param shell Structure principale du shell
 * @return Code de sortie de la commande exécutée
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
	close_all_heredoc_fds(shell);
	free_all_heredoc_fds(shell);
	return (handle_child_status(status));
}

/**
 * @brief Exécute une commande dans un processus enfant sans gestion des heredocs.
 *
 * Fork un nouveau processus, exécute la commande (avec redirections mais sans heredocs),
 * gère les signaux, attend la fin du processus et retourne le code de sortie.
 *
 * @param cmd_node Nœud AST de la commande à exécuter
 * @param env Liste chaînée des variables d'environnement
 * @param ast_root AST racine (pour les redirections)
 * @param shell Structure principale du shell
 * @return Code de sortie de la commande exécutée
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
	return (handle_child_status(status));
}
