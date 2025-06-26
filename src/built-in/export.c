/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 18:55:13 by elaudrez          #+#    #+#             */
/*   Updated: 2025/06/26 15:51:45 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Exporter des variables
Definir une variable qui ne serait dispo que dans shell courant ex : VAR=25
Export va l'integrer aux variables d'env qui se trouve dans **env
*/

void	add_new_node(t_env *new_node, t_env **env)
{
	t_env	*curr;

	if (!*env)
		*env = new_node;
	else
	{
		curr = *env;
		while(curr->next)
			curr = curr->next;
		curr->next = new_node;
	}
}

void	create_add_new_node(char *key, char *value, t_env **env)
{
	t_env	*new_node;
	
	new_node = ft_calloc(1, sizeof(t_env));
	if (!new_node)
		return ;
	new_node->key = ft_strdup(key);
	new_node->value = ft_strdup(value);
	new_node->next = NULL;
	add_new_node(new_node, env);
}

int	update_env_value(t_env *env, char *key, char *value)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
		{
			free(env->value);
			env->value = ft_strdup(value);
			return (1);
		}
		env = env->next;
	}
	return (0);
}

int	ft_export(t_ast *node, t_shell *data)
{
	int	i;
	int	j;
	char	*key;
	char	*value;
	
	i = 1;
	while(node->args[i])
	{
		j = 0;
		if(!ft_strchr(node->args[i], '='))
			return (0);
		while (node->args[i][j] && node->args[i][j] != '=')
			j++;
		key = ft_substr(node->args[i], 0, j);
		value = ft_substr(node->args[i], j + 1, ft_strlen(node->args[i]) - (j + 1));
		if (!update_env_value(data->env_list, key, value) && ft_is_valid(key))
			create_add_new_node(key, value, &data->env_list);
		free(key);
		free(value);
		i++;
	}
	return (0);
}