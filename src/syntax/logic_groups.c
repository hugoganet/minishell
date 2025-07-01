/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logic_groups.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 19:48:36 by elaudrez          #+#    #+#             */
/*   Updated: 2025/05/28 17:33:58 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token *is_logic(t_token **node)
{
	t_token *ptr;

	ptr = *node;
	// If the first token is a WORD, we assume it's a command
	if (ptr != NULL && ptr->type == WORD)
	{
		ptr->type = CMD;
		// ! On ne passe pas au token suivant, car CMD peut être suivi de plusieurs ARG
		ptr = ptr->next;
	}
	// Iterate through the list to set types for logical operators and redirections
	while (ptr != NULL)
	{

		if (ptr->type == PIPE && ptr->next)
		{
			ptr = ptr->next;
			ptr->type = CMD;
		}
		else if ((ptr->type == REDIR_INPUT || ptr->type == REDIR_APPEND ||
				  ptr->type == REDIR_OUTPUT || ptr->type == HEREDOC) &&
				 ptr->next)
		{
			ptr = ptr->next;
			ptr->type = FILES;
		}
		ptr = ptr->next;
	}
	// After setting the types for logical operators and redirections,
	// we iterate again to set the type for arguments following commands
	ptr = *node;
	while (ptr != NULL)
	{
		if (ptr->type == CMD && ptr->next->type == WORD)
		{
			ptr = ptr->next;
			ptr->type = ARG;
		}
		ptr = ptr->next;
	}
	return (*node);
}
