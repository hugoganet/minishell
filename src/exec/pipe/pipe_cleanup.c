/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_cleanup.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 16:00:00 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/07 16:00:00 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipe.h"

void	cleanup_pipeline_resources(t_ast **commands, int **pipes,
			pid_t *pids, int pipes_created);
void	cleanup_pipeline_memory_only(t_ast **commands, int **pipes,
			pid_t *pids, int pipes_created);
void	cleanup_child_memory_early(t_ast **commands, int **pipes,
			pid_t *pids, int pipes_created);
void	terminate_child_processes(pid_t *pids, int count);
void	initialize_pipeline_pids(pid_t *pids, int cmd_count);

void	cleanup_pipeline_resources(t_ast **commands, int **pipes,
			pid_t *pids, int pipes_created)
{
	int	i;

	i = 0;
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
	if (commands)
		free(commands);
	if (pids)
		free(pids);
}

void	cleanup_pipeline_memory_only(t_ast **commands, int **pipes,
			pid_t *pids, int pipes_created)
{
	int	i;

	i = 0;
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
	if (commands)
		free(commands);
	if (pids)
		free(pids);
}

void	cleanup_child_memory_early(t_ast **commands, int **pipes,
			pid_t *pids, int pipes_created)
{
	int	i;

	i = 0;
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
	if (commands)
		free(commands);
	if (pids)
		free(pids);
}

void	terminate_child_processes(pid_t *pids, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if (pids[i] > 0)
			kill(pids[i], SIGTERM);
		i++;
	}
	i = 0;
	while (i < count)
	{
		if (pids[i] > 0)
			waitpid(pids[i], NULL, 0);
		i++;
	}
}

void	initialize_pipeline_pids(pid_t *pids, int cmd_count)
{
	int	i;

	i = 0;
	while (i < cmd_count)
	{
		pids[i] = -1;
		i++;
	}
}
