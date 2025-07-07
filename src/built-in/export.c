/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 18:55:13 by elaudrez          #+#    #+#             */
/*   Updated: 2025/07/07 13:50:13 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	update_existing_env_value(t_env *env, char *key, char *value)
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

static void	upadate_or_add_env(t_shell *data, char *key, char *value)
{
	if (!update_existing_env_value(data->env_list, key, value)
		&& ft_is_valid(key))
		create_add_new_node(key, value, &data->env_list);
	if (!update_existing_env_value(data->export_list, key, value)
		&& ft_is_valid(key))
	{
		create_add_new_node(key, value, &data->export_list);
		sort_list(&data->export_list);
	}
}

static int	export_without_value(char *arg, t_shell *data)
{
	if (ft_is_valid(arg))
	{
		create_add_new_node(arg, NULL, &data->export_list);
		sort_list(&data->export_list);
		return (0);
	}
	return (handle_invalid_export(arg));
}

static int	export_equal(char *arg, t_shell *data)
{
	int		j;
	char	*key;
	char	*value;

	j = 0;
	while (arg[j] && arg[j] != '=')
		j++;
	key = ft_substr(arg, 0, j);
	if (!ft_is_valid(key))
	{
		free(key);
		return (handle_invalid_export(arg));
	}
	value = ft_substr(arg, j + 1, ft_strlen(arg) - (j + 1));
	upadate_or_add_env(data, key, value);
	free(key);
	free(value);
	return (0);
}

int	ft_export(t_ast *node, t_shell *data)
{
	int	i;
	int	error_status;

	i = 1;
	error_status = 0;
	if (!node->args[1])
	{
		print_export_list(data->export_list);
		return (0);
	}
	while (node->args[i])
	{
		if (ft_strchr(node->args[i], '='))
			error_status = export_equal(node->args[i], data);
		else
			error_status = export_without_value(node->args[i], data);
		i++;
	}
	return (error_status);
}
