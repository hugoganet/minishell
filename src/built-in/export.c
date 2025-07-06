/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 18:55:13 by elaudrez          #+#    #+#             */
/*   Updated: 2025/07/06 16:38:31 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int update_existing_env_value(t_env *env, char *key, char *value)
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

static void upadate_or_add_env(t_shell *data, char *key, char *value)
{
	if (!update_existing_env_value(data->env_list, key, value) && ft_is_valid(key))
		create_add_new_node(key, value, &data->env_list);
	if (!update_existing_env_value(data->export_list, key, value) && ft_is_valid(key))
	{
		create_add_new_node(key, value, &data->export_list);
		sort_list(&data->export_list);
	}
}

static int export_equal(char *arg, t_shell *data)
{
	int j;
	char *key;
	char *value;

	j = 0;
	while (arg[j] && arg[j] != '=')
		j++;
	key = ft_substr(arg, 0, j);
	if (!ft_is_valid(key))
	{
		ft_putstr_fd("Minishell: export: ", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(" not a valid identifier\n", 2);
		free(key);
		return (1);
	}
	value = ft_substr(arg, j + 1, ft_strlen(arg) - (j + 1));
	upadate_or_add_env(data, key, value);
	free(key);
	free(value);
	return (0);
}

int ft_export(t_ast *node, t_shell *data)
{
	int i;
	int error_status;

	i = 1;
	error_status = 0;
	if (!node->args[1])
		print_export_list(data->export_list);
	while (node->args[i])
	{
		if (!ft_strchr(node->args[i], '=') && ft_is_valid(node->args[i]))
		{
			create_add_new_node(node->args[i], NULL, &data->export_list);
			sort_list(&data->export_list);
		}
		else if (!ft_strchr(node->args[i], '=') && !ft_is_valid(node->args[i]))
		{
			ft_putstr_fd("Minishell: export: ", 2);
			ft_putstr_fd(node->args[i], 2);
			ft_putstr_fd(" not a valid identifier\n", 2);
			error_status = 1;
		}
		else if (export_equal(node->args[i], data))
			error_status = 1;
		i++;
	}
	return (error_status);
}
