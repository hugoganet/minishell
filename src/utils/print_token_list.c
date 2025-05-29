/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_token_list.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 18:19:01 by hugoganet         #+#    #+#             */
/*   Updated: 2025/05/29 08:57:25 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static const char *token_type_str(t_token_type type)
{
	if (type == WORD)
		return ("WORD");
	if (type == CMD)
		return ("CMD");
	if (type == ARG)
		return ("ARG");
	if (type == FILES)
		return ("FILES");
	if (type == PIPE)
		return ("PIPE");
	if (type == REDIR_INPUT)
		return ("REDIR_INPUT");
	if (type == REDIR_OUTPUT)
		return ("REDIR_OUTPUT");
	if (type == REDIR_APPEND)
		return ("REDIR_APPEND");
	if (type == HEREDOC)
		return ("HEREDOC");
	if (type == AND)
		return ("AND");
	if (type == OR)
		return ("OR");
	if (type == PAREN_LEFT)
		return ("PAREN_LEFT");
	if (type == PAREN_RIGHT)
		return ("PAREN_RIGHT");
	return ("UNKNOWN");
}

/**
 * @brief Affiche chaque token de la liste avec son type.
 *
 * @param tokens Liste chaînée de tokens
 */
void print_token_list(t_token *tokens)
{
	printf("\n=== Token List ===\n");
	while (tokens)
	{
		printf(" - %-10s → %s\n", tokens->str, token_type_str(tokens->type));
		tokens = tokens->next;
	}
	printf("==================\n\n");
}
