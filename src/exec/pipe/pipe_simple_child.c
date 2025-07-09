/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_simple_child.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 16:00:00 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/07 16:00:00 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipe.h"

/**
 * 
 */
void	setup_left_child_process(t_simple_pipe_ctx *ctx)
{
	int	status;

	reset_signals_in_child();
	signal(SIGPIPE, SIG_DFL);
	setup_heredoc_redirection(ctx->shell);
	if (dup2(ctx->fd[1], STDOUT_FILENO) == -1)
	{
		perror("minishell: dup2");
		cleanup_shell(ctx->shell);
		exit(1);
	}
	close(ctx->fd[0]);
	close(ctx->fd[1]);
	close_all_heredoc_fds(ctx->shell);
	status = exec_cmd_no_heredoc(ctx->node->left, ctx->env, ctx->node->left,
			ctx->shell);
	cleanup_shell(ctx->shell);
	exit(status);
}

void	setup_right_child_process(t_simple_pipe_ctx *ctx)
{
	int	status;

	reset_signals_in_child();
	signal(SIGPIPE, SIG_DFL);
	if (dup2(ctx->fd[0], STDIN_FILENO) == -1)
	{
		perror("minishell: dup2");
		cleanup_shell(ctx->shell);
		exit(1);
	}
	close(ctx->fd[0]);
	close(ctx->fd[1]);
	close_all_heredoc_fds(ctx->shell);
	status = exec_cmd_no_heredoc(ctx->node->right, ctx->env, ctx->node->right,
			ctx->shell);
	cleanup_shell(ctx->shell);
	exit(status);
}

int	wait_for_children(pid_t left_pid, pid_t right_pid)
{
	int	status;

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

int	handle_pipe_execution_error(int fd[2], pid_t left_pid)
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
