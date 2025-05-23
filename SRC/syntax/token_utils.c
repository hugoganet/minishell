/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 17:26:12 by hugoganet         #+#    #+#             */
/*   Updated: 2025/05/23 17:31:24 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Associe une chaîne à son type de token.
 *
 * @param str Token brut
 * @return t_token_type Type du token
 */
t_token_type get_token_type(char *str)
{
	if (!str)
		return (TOK_WORD);
	if (!ft_strncmp(str, "|", 2))
		return (TOK_PIPE);
	if (!ft_strncmp(str, "<", 2))
		return (TOK_REDIR_IN);
	if (!ft_strncmp(str, ">", 2))
		return (TOK_REDIR_OUT);
	if (!ft_strncmp(str, "<<", 3))
		return (TOK_REDIR_HEREDOC);
	if (!ft_strncmp(str, ">>", 3))
		return (TOK_REDIR_APPEND);
	if (!ft_strncmp(str, "(", 2))
		return (TOK_PAREN_LEFT);
	if (!ft_strncmp(str, ")", 2))
		return (TOK_PAREN_RIGHT);
	return (TOK_WORD);
}