/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_complex_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 16:00:00 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/07 16:00:00 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipe.h"

void	setup_child_context(t_pipeline_ctx *ctx, t_pipeline_params *params)
{
	ctx->commands = params->commands;
	ctx->pipes = params->pipes;
	ctx->pids = params->pids;
	ctx->cmd_count = params->cmd_count;
	ctx->index = params->index;
	ctx->env = params->env;
	ctx->shell = params->shell;
}

int	create_child_process(t_pipeline_params *params)
{
	t_pipeline_ctx	ctx;

	params->pids[params->index] = fork();
	if (params->pids[params->index] < 0)
	{
		perror("minishell: fork");
		terminate_child_processes(params->pids, params->index);
		return (1);
	}
	if (params->pids[params->index] == 0)
	{
		setup_child_context(&ctx, params);
		execute_pipeline_child(&ctx);
	}
	return (0);
}

int	create_pipeline_processes(t_pipeline_params *params)
{
	int	i;

	initialize_pipeline_pids(params->pids, params->cmd_count);
	i = 0;
	while (i < params->cmd_count)
	{
		params->index = i;
		if (create_child_process(params) != 0)
			return (1);
		i++;
	}
	return (0);
}

int	wait_for_all_processes(pid_t *pids, int cmd_count)
{
	int	i;
	int	status;
	int	final_status;

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
