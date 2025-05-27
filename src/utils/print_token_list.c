/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_token_list.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 18:19:01 by hugoganet         #+#    #+#             */
/*   Updated: 2025/05/27 18:32:19 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static const char *token_type_str(t_token_type type)
{
	if (type == TOK_WORD)
		return ("TOK_WORD");
	if (type == TOK_PIPE)
		return ("TOK_PIPE");
	if (type == TOK_REDIR_IN)
		return ("TOK_REDIR_IN");
	if (type == TOK_REDIR_OUT)
		return ("TOK_REDIR_OUT");
	if (type == TOK_REDIR_APPEND)
		return ("TOK_REDIR_APPEND");
	if (type == TOK_REDIR_HEREDOC)
		return ("TOK_REDIR_HEREDOC");
	if (type == TOK_AND)
		return ("TOK_AND");
	if (type == TOK_OR)
		return ("TOK_OR");
	if (type == TOK_PAREN_LEFT)
		return ("TOK_PAREN_LEFT");
	if (type == TOK_PAREN_RIGHT)
		return ("TOK_PAREN_RIGHT");
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
		printf(" - %-10s → %s\n", tokens->value, token_type_str(tokens->type));
		tokens = tokens->next;
	}
	printf("==================\n\n");
}
