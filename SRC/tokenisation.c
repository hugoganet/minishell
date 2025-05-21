/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenisation.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 13:28:30 by elaudrez          #+#    #+#             */
/*   Updated: 2025/05/21 12:06:27 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	main()
{
	char	*rl;
	rl = readline("Minishell > ");
	printf("%s\n", rl);
	return (0);
}



