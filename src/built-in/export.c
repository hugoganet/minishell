/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 18:55:13 by elaudrez          #+#    #+#             */
/*   Updated: 2025/06/25 18:10:49 by elaudrez         ###   ########.fr       */
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

int	ft_export(t_ast *node, t_shell *data)
{
	int	i;
	int	j;
	t_env	*new_node;
	t_env	*curr;
	char	*key;

	i = 1;
	j = 0;
	new_node = ft_calloc(1, sizeof(t_env));
	if (!new_node)
		return (1);
	if (data->env_list)
		curr = data->env_list;
	else 
	
	while(node->args[i])
	{
		while (node->args[i][j] != '=')
			j++;
		key = ft_substr(node->args[i], 0, j - 1)
		while (curr)
		{
			if (ft_strcmp(curr, key) == 0)
			{
				//remplacer la value par le reste dond de j a \0.
			}
			curr = curr->next;
		}
		else
		{
			if (ft_is_valid(key))
			{
				new_node->key = ft_substr(node->args[i], node->args[i][0], j - 1);
				new_node->value = ft_substr(node->args[i], j + 1, ft_strlen(node->args[i]) - (j + 1));
				new_node->next = NULL;
				add_new_node(new_node, &data->env_list)
			}
		}
		
	}
}