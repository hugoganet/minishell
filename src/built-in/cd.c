/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 16:59:32 by elaudrez          #+#    #+#             */
/*   Updated: 2025/06/25 11:10:27 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_env(t_env **env, char *key, char *value)
{
	// int		i;
	t_env	*new_var;
	t_env	*tmp;

	// i = 0;
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

int	ft_cd(t_ast *node, t_shell *data)
{
	char	*oldpwd;
	char	*newpwd;
 
	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
	{
		free(oldpwd);
		return (1);
	}
	if (chdir(node->args[1])  == -1)
	{
		perror("bash: cd: no such file or directory ");
		return (EXIT_FAILURE);
	}
	newpwd = getcwd(NULL, 0);
	
	if (!newpwd)
	{
		free(newpwd);
		return (1);
	}
	update_env(&data->env_list, "OLDPWD", oldpwd);
	update_env(&data->env_list, "PWD", newpwd);
	free(oldpwd);
	free(newpwd);
	return(0);
}
