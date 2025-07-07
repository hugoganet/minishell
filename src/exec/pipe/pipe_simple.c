/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_simple.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:03:03 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/07 11:50:55 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipe.h"

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
int execute_simple_pipe(t_ast *node, t_env *env, t_shell *shell)
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

int process_simple_pipe_heredocs(t_ast *node, t_shell *shell)
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

int execute_simple_pipe_processes(int fd[2], t_ast *node, t_env *env, t_shell *shell)
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

int execute_left_pid(int fd[2], pid_t *left_pid, t_ast *node, t_env *env, t_shell *shell)
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

int execute_right_pid(int fd[2], pid_t *right_pid, t_ast *node, t_env *env, t_shell *shell)
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