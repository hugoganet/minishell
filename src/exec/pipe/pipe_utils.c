/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 16:00:00 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/07 16:00:00 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipe.h"

int		count_pipeline_commands(t_ast *node);
void	extract_pipeline_commands(t_ast *node, t_ast **commands, int *index);
bool	is_complex_pipeline(t_ast *node);
int		allocate_pipeline_resources(int cmd_count, t_ast ***commands,
			int ***pipes, pid_t **pids);
int		create_all_pipes(int **pipes, int cmd_count);

int	count_pipeline_commands(t_ast *node)
{
	if (!node)
		return (0);
	if (node->type == CMD)
		return (1);
	if (node->type == PIPE)
		return (count_pipeline_commands(node->left)
			+ count_pipeline_commands(node->right));
	if (find_cmd_node(node))
		return (1);
	return (0);
}

void	extract_pipeline_commands(t_ast *node, t_ast **commands, int *index)
{
	t_ast	*cmd_node;

	if (!node)
		return ;
	if (node->type == PIPE)
	{
		extract_pipeline_commands(node->left, commands, index);
		extract_pipeline_commands(node->right, commands, index);
		return ;
	}
	if (node->type == CMD)
	{
		commands[*index] = node;
		(*index)++;
		return ;
	}
	cmd_node = find_cmd_node(node);
	if (cmd_node)
	{
		commands[*index] = node;
		(*index)++;
	}
}

bool	is_complex_pipeline(t_ast *node)
{
	return (count_pipeline_commands(node) > 2);
}

int	allocate_pipeline_resources(int cmd_count, t_ast ***commands,
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

int	create_all_pipes(int **pipes, int cmd_count)
{
	int	i;

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
