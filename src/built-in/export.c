/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 18:55:13 by elaudrez          #+#    #+#             */
/*   Updated: 2025/06/25 12:48:03 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Exporter des variables
Definir une variable qui ne serait dispo que dans shell courant ex : VAR=25
Export va l'integrer aux variables d'env qui se trouve dans **env
*/


// int	ft_export(t_ast *node, t_shell *data)
// {
// 	int	i;
// 	int	j;
// 	t_env	*new_node;

// 	i = 1;
// 	j = 0;
// 	new_node = ft_calloc(1, sizeof(t_env));
// 	if (!new_node)
// 		return (1);
// 	while(node->args[i])
// 	{
// 		while (node->args[i][j] != '=')
// 			j++;
// 		new_node->key = ft_str(node->args[i][0], node->args[i][j - 1]);
// 		new_node->value = ft_str(node->args[i][j + 1], node->args[i][ft_strlen(node->args[i])]);
// 	}
// }