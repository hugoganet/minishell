/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expand.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 19:54:08 by elaudrez          #+#    #+#             */
/*   Updated: 2025/05/29 18:37:27 by elaudrez         ###   ########.fr       */
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

void	exec_expand(t_ast *node, t_shell *data)
{
	int	i;
	int	j;
	int	len;
	char *name_var;

	i = 0;
	while(node->str[i] && node->str[i] != '$')
		i++;
	i++;
	j = i;
	while (node->str[] < 65 || node->str[i] > 90)
	
	
	while (data->env[i])
	{
		if (ft_strncmp(data->env[j], name_var, len) == 0)
			break;
		j++;
	}
	
}

int	which_quote(t_node *node)
{
	if (node->str[0] == 34)
		return (1);
	else if (node->str[0] == 39)
		return (2);
	else
		return (3);
}

void	expand_vars(t_ast *node)
{
	
	if (node->type == ARG)
	{
		if (ft_strchr(node->str, '$') == '$' && );
		{
			if (which_quote(node) == 2)
				passer
			else
				exec_expand(node);
		}
	}
	travel_in_ast(node);
	
}