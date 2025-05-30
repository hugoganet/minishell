/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expand.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 19:54:08 by elaudrez          #+#    #+#             */
/*   Updated: 2025/05/30 16:16:39 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Boucler en recursif sur l'ast : 
	descendre tout en bas avec travel_in_ast()
	Puis remonter en executant
	*/

void	travel_in_ast(t_ast *node)
{
	if (!node)
		return;
	travel_in_ast(node->left);
	travel_in_ast(node->right);
}

char	*find_var(t_ast *node)
{
	int	i;
	int	j;
	int	len;
	char *name_var;

	i = 0;
	while(node->str[i] && node->str[i] != '$')
		i++;
	if (node->str[i] == '\0')
		return (NULL);
	else
		i++;
	if (node->str[i] == '{')
		i++;
	j = 0;
	if (node->str[i] < 65 || node->str[i] > 90)
	{
		while (node->str[i] < 65 || node->str[i] > 90)
		{
			name_var[j] = node->str[i];
			i++;
			j++;
		}
		name_var[j] = '\0';
	}
	else
		return (NULL);
	return (name_var);
}

char	*copy_var(t_shell *data, int i, int len)
{
	int j;
	char *var;

	var = ft_strcpy(var, data->env[i][len + 1]);
	return (var);
}

char	*exec_expand(t_ast *node, t_shell *data)
{
	int		i;
	char	*name_var;
	int		len;
	char	*var;
	
	i = 0;
	name_var = find_var(node);
	len = ft_strlen(name_var);
	while (data->env[i])
	{
		if (ft_strncmp(data->env[i], name_var, len) == 0)
			break;
		i++;
	}
	if (data->env[i] == NULL)
		return (NULL);
	var = copy_var(data, i, len);
	return (var);
}

int	which_quote(t_ast *node)
{
	if (node->str[0] == 34)
		return (1);
	else if (node->str[0] == 39)
		return (2);
	else
		return (3);
}

void	expand_vars(t_ast *node, t_shell *data)
{
	if (node->type == ARG)
	{
		if (ft_strchr(node->str, '$') == '$');
		{
			if (which_quote(node) == 1)
			{
				exec_expand(node, data);
			}
			
		}
	}
	travel_in_ast(node);
}