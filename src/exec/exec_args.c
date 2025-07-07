/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_args.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 08:32:17 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/06 20:47:47 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"

int	validate_command(t_ast *cmd_node)
{
	if (!cmd_node || !cmd_node->args)
		return (0);
	if (!cmd_node->args[0])
		return (0);
	if (cmd_node->args[0][0] == '\x01' && cmd_node->args[0][1] == '\0')
		return (0);
	if (cmd_node->args[0][0] == '\0')
		return (-1);
	return (1);
}

void	filter_empty_args(char **args)
{
	int	read_idx;
	int	write_idx;

	if (!args)
		return ;
	read_idx = 0;
	write_idx = 0;
	while (args[read_idx])
	{
		if (args[read_idx][0] == '\x01' && args[read_idx][1] == '\0')
		{
			free(args[read_idx]);
		}
		else
		{
			if (write_idx != read_idx)
				args[write_idx] = args[read_idx];
			write_idx++;
		}
		read_idx++;
	}
	args[write_idx] = NULL;
}
