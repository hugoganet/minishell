/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 15:48:59 by elaudrez          #+#    #+#             */
/*   Updated: 2025/07/04 15:51:18 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_new_node(t_env *new_node, t_env **env)
{
	t_env	*curr;

	if (!*env)
		*env = new_node;
	else
	{
		curr = *env;
		while (curr->next)
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
	new_node->value = value ? ft_strdup(value) : NULL;
	new_node->next = NULL;
	add_new_node(new_node, env);
}

void	print_export_list(t_env *export_list)
{
	t_env	*curr;

	curr = export_list;
	while (curr)
	{
		printf("export ");
		if (curr->value == NULL)
			printf("%s\n", curr->key);
		else
		{
			printf("%s", curr->key);
			printf("=");
			printf("\"%s\"\n", curr->value);
		}
		curr = curr->next;
	}
}

void	ft_swap(t_env *i, t_env *j)
{
	char	*tmp_key;
	char	*tmp_value;

	tmp_key = i->key;
	i->key = j->key;
	j->key = tmp_key;
	tmp_value = i->value;
	i->value = j->value;
	j->value = tmp_value;
}

void	sort_list(t_env **export_list)
{
	t_env	*i;
	t_env	*j;

	i = *export_list;
	if (!export_list)
		return ;
	while (i)
	{
		j = i->next;
		while (j)
		{
			if (ft_strcmp(i->key, j->key) > 0)
			{
				ft_swap(i, j);
				continue ;
			}
			j = j->next;
		}
		i = i->next;
	}
}