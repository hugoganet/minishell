/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 16:20:33 by hugoganet         #+#    #+#             */
/*   Updated: 2025/05/23 16:20:45 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Vérifie si la syntaxe globale de l’entrée utilisateur est correcte.
 *
 * @param input Ligne à analyser
 * @return int 0 si tout est bon, 1 s'il y a une erreur de syntaxe
 */
int is_syntax_valid(char *input)
{
	if (has_unclosed_quotes(input))
	{
		ft_putendl_fd("minishell: syntax error: unclosed quote", 2);
		return (1);
	}
	if (has_invalid_pipes(input))
	{
		ft_putendl_fd("minishell: syntax error near unexpected token `|'", 2);
		return (1);
	}
	if (has_invalid_redirections(input))
	{
		ft_putendl_fd("minishell: syntax error near redirection", 2);
		return (1);
	}
	if (has_unmatched_parentheses(input))
	{
		ft_putendl_fd("minishell: syntax error: unmatched or empty parentheses", 2);
		return (1);
	}
	return (0);
}