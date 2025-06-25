/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 14:08:23 by elaudrez          #+#    #+#             */
/*   Updated: 2025/06/25 11:33:00 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(t_ast *node)
{
	if (ft_strcmp(node->args[0], "echo") == 0
			|| ft_strcmp(node->args[0], "env") == 0
			|| ft_strcmp(node->args[0], "cd") == 0
			|| ft_strcmp(node->args[0], "unset") == 0
			|| ft_strcmp(node->args[0], "pwd") == 0
			// || ft_strcmp(node->args[0], "export") == 0
			|| ft_strcmp(node->args[0], "exit") == 0)
		return (1);
	return (0);
}

int	builtin_exec(t_ast *node, t_shell *data)
{
	if (ft_strcmp(node->args[0], "echo") == 0)
		return (ft_echo(node));
	else if (ft_strcmp(node->args[0], "env") == 0)
		return (ft_env(node, data));
	else if (ft_strcmp(node->args[0], "cd") == 0)
		return (ft_cd(node, data));
	else if (ft_strcmp(node->args[0], "unset") == 0)
		return (ft_unset(node, data));
	else if (ft_strcmp(node->args[0], "pwd") == 0)
		return (ft_pwd());
	else if (ft_strcmp(node->args[0], "exit") == 0)
		return (ft_exit(node, data));
	// else if (ft_strcmp(node->args[0], "export") == 0)
	// 	return (ft_export(node, data));
	return (1);
}
