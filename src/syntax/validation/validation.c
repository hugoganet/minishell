/* ***************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 16:20:33 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/02 16:01:43 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "syntax.h"

/**
 * @brief Vérifie si la syntaxe globale de l’entrée utilisateur est correcte.
 *
 * Affecte last_exit_status à 2 en cas d’erreur.
 *
 * @param input Ligne à analyser
 * @return int 0 si tout est bon, 1 s'il y a une erreur de syntaxe
 */
int is_syntax_valid(char *input, t_shell *shell)
{
	if (has_unclosed_quotes(input))
	{
		ft_putendl_fd("minishell: syntax error: unclosed quote", 2);
		shell->last_exit_status = 2; // Set last exit status to 2 for unclosed quotes
		return (1);
	}
	if (has_invalid_pipes(input))
	{
		ft_putendl_fd("minishell: syntax error near unexpected token `|'", 2);
		shell->last_exit_status = 2; // Set last exit status to 2 for invalid pipes
		return (1);
	}
	if (has_invalid_redirections(input))
	{
		ft_putendl_fd("minishell: syntax error near unexpected token 'redirection'", 2);
		shell->last_exit_status = 2; // Set last exit status to 2 for invalid redirections
		return (1);
	}
	if (has_unclosed_braces(input))
	{
		ft_putendl_fd("minishell: syntax error : unclosed braces", 2);
		shell->last_exit_status = 2; // Set last exit status to 2 for invalid redirections
		return (1);
	}
	return (0);
}
