/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenisation.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 14:51:50 by elaudrez          #+#    #+#             */
/*   Updated: 2025/05/21 16:56:07 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Crée un nouveau nœud de liste chaînée.
 *
 * Alloue dynamiquement un élément de type `t_list` et y affecte les données fournies.
 *
 * @param str Une chaîne associée au nœud (non dupliquée, le pointeur est conservé tel quel).
 * @param type Une valeur de type `t_enum` définissant le type de ce nœud.
 * @return Un pointeur vers le nouveau nœud, ou NULL en cas d’erreur d’allocation.
 *
 * @warning Le contenu de `str` n'est pas dupliqué. Il doit rester valide tant que le nœud existe.
 */
t_list	*create_node(char *str, t_enum type)
{
	t_list	*new_node;
	
	new_node = malloc(sizeof(new_node));
	if (!new_node)
		return (NULL);
	new_node->type = type;
	new_node->str = str;
	new_node->next = NULL;
	return (new_node);
}

// void	tokenizer(char *str)
// {
// 	int	i;

// 	i = 0;
// 	while (str[i])
// 	{
		
// 	}
// }