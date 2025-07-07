/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_complex.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 16:00:00 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/07 17:00:51 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipe.h"

static int	execute_pipeline_setup(t_ast *node, t_pipeline_data *data,
		t_shell *shell)
{
	int	setup_result;

	data->commands = NULL;
	data->pipes = NULL;
	data->pids = NULL;
	setup_result = setup_pipeline_execution(node, data, shell);
	if (setup_result != 0)
		return (setup_result);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	return (0);
}

static int	execute_pipeline_process(t_ast *node, t_pipeline_data *data,
		t_env *env, t_shell *shell)
{
	t_pipeline_params	params;
	int					setup_result;

	params.commands = data->commands;
	params.pipes = data->pipes;
	params.pids = data->pids;
	params.cmd_count = count_pipeline_commands(node);
	params.env = env;
	params.shell = shell;
	setup_result = create_pipeline_processes(&params);
	if (setup_result != 0)
	{
		cleanup_pipeline_resources(data->commands, data->pipes, data->pids,
			data->pipes_created);
		init_signals();
		return (1);
	}
	return (0);
}

int	execute_complex_pipeline(t_ast *node, t_env *env, t_shell *shell)
{
	t_pipeline_data	data;
	int				setup_result;

	setup_result = execute_pipeline_setup(node, &data, shell);
	if (setup_result != 0)
		return (setup_result);
	setup_result = execute_pipeline_process(node, &data, env, shell);
	if (setup_result != 0)
		return (setup_result);
	close_parent_pipes(data.pipes, count_pipeline_commands(node));
	setup_result = wait_for_all_processes(data.pids,
			count_pipeline_commands(node));
	init_signals();
	close_all_heredoc_fds(shell);
	free_all_heredoc_fds(shell);
	cleanup_pipeline_memory_only(data.commands, data.pipes, data.pids,
		data.pipes_created);
	return (setup_result);
}

static int	setup_pipeline_resources(t_ast *node, t_pipeline_data *data)
{
	int	cmd_count;

	cmd_count = count_pipeline_commands(node);
	if (cmd_count <= 0)
		return (1);
	if (allocate_pipeline_resources(cmd_count, &data->commands,
			&data->pipes, &data->pids) != 0)
		return (1);
	data->pipes_created = create_all_pipes(data->pipes, cmd_count);
	if (data->pipes_created != cmd_count - 1)
	{
		cleanup_pipeline_resources(data->commands, data->pipes,
			data->pids, data->pipes_created);
		return (1);
	}
	return (0);
}

int	setup_pipeline_execution(t_ast *node, t_pipeline_data *data,
		t_shell *shell)
{
	int	index;
	int	cmd_count;

	if (setup_pipeline_resources(node, data) != 0)
		return (1);
	index = 0;
	cmd_count = count_pipeline_commands(node);
	extract_pipeline_commands(node, data->commands, &index);
	if (process_all_heredocs(data->commands, cmd_count, shell) == 130)
	{
		cleanup_pipeline_resources(data->commands, data->pipes,
			data->pids, data->pipes_created);
		return (130);
	}
	return (0);
}
