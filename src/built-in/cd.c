/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 16:59:32 by elaudrez          #+#    #+#             */
/*   Updated: 2025/07/04 15:08:15 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_env(t_env **env, char *key, char *value)
{
	t_env	*new_var;
	t_env	*tmp;

	tmp = *env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
		{
			free(tmp->value);
			tmp->value = ft_strdup(value);
			return ;
		}
		tmp = tmp->next;
	}
	new_var = malloc(sizeof(t_env));
	if (!new_var)
		return ;
	new_var->key = ft_strdup(key);
	new_var->value = ft_strdup(value);
	new_var->next = *env;
	*env = new_var;
}

void	exec_update_env(t_shell *data, char **oldpwd, char **newpwd)
{
	update_env(&data->env_list, "OLDPWD", *oldpwd);
	update_env(&data->export_list, "OLDPWD", *oldpwd);
	update_env(&data->env_list, "PWD", *newpwd);
	update_env(&data->export_list, "PWD", *newpwd);
}

int	exec_cd(t_ast *node, t_shell *data)
{
	char	*oldpwd;
	char	*newpwd;

	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
		return (1);
	if (chdir(node->args[1]) == -1)
	{
		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
		perror(node->args[1]);
		free(oldpwd);
		return (EXIT_FAILURE);
	}
	newpwd = getcwd(NULL, 0);
	if (!newpwd)
	{
		free(oldpwd);
		return (1);
	}
	exec_update_env(data, &oldpwd, &newpwd);
	free(oldpwd);
	free(newpwd);
	return (0);
}

int	ft_cd(t_ast *node, t_shell *data)
{
	if (!node->args[1])
	{
		ft_putstr_fd("Subject : cd with only a relative or absolute path\n",
			STDERR_FILENO);
		return (1);
	}
	if (node->args[2])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", STDERR_FILENO);
		return (1);
	}
	if (exec_cd(node, data))
		return (1);
	return (0);
}
