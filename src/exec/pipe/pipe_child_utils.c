/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_child_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 16:00:00 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/07 16:00:00 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipe.h"

void	setup_child_pipes(int **pipes, int cmd_count, int child_index,
			t_shell *shell);
void	setup_child_stdin(int **pipes, int child_index, t_shell *shell);
void	setup_child_stdout(int **pipes, int child_index, int cmd_count,
			t_shell *shell);
void	close_all_child_pipes(int **pipes, int cmd_count);
void	execute_pipeline_child(t_pipeline_ctx *ctx);

void	setup_child_pipes(int **pipes, int cmd_count, int child_index,
			t_shell *shell)
{
	setup_child_stdin(pipes, child_index, shell);
	setup_child_stdout(pipes, child_index, cmd_count, shell);
	close_all_child_pipes(pipes, cmd_count);
}

void	setup_child_stdin(int **pipes, int child_index, t_shell *shell)
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

void	setup_child_stdout(int **pipes, int child_index, int cmd_count,
			t_shell *shell)
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

void	close_all_child_pipes(int **pipes, int cmd_count)
{
	int	j;

	j = 0;
	while (j < cmd_count - 1)
	{
		close(pipes[j][0]);
		close(pipes[j][1]);
		j++;
	}
}

void	execute_pipeline_child(t_pipeline_ctx *ctx)
{
	t_ast	*current_cmd;
	int		status;

	reset_signals_in_child();
	signal(SIGPIPE, SIG_DFL);
	setup_heredoc_redirection(ctx->shell);
	current_cmd = ctx->commands[ctx->index];
	setup_child_pipes(ctx->pipes, ctx->cmd_count, ctx->index, ctx->shell);
	close_all_heredoc_fds(ctx->shell);
	cleanup_child_memory_early(ctx->commands, ctx->pipes, ctx->pids,
		ctx->cmd_count - 1);
	status = exec_cmd_no_heredoc(current_cmd, ctx->env, current_cmd,
			ctx->shell);
	cleanup_shell(ctx->shell);
	exit(status);
}
