/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 15:06:35 by elaudrez          #+#    #+#             */
/*   Updated: 2025/07/09 18:29:01 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "syntax.h"

/**
 * @brief Vérifie si la syntaxe globale de l’entrée utilisateur est correcte.
 * 
 * - Quotes non fermées
 * 
 * - Pipes invalides (en début ou fin de ligne, ou deux pipes consécutifs)
 * 
 * - Redirections invalides (en début ou fin de ligne, ou deux redirections
 * consécutives)
 * 
 * - Accolades non fermées
 *
 * Affecte last_exit_status à 2 en cas d’erreur.
 *
 * @param input Ligne à analyser
 * @return int 0 si tout est bon, 1 s'il y a une erreur de syntaxe
 */
int	is_syntax_valid(char *input, t_shell *shell)
{
	if (has_unclosed_quotes(input))
	{
		ft_putendl_fd("minishell: syntax error: unclosed quote", 2);
		shell->last_exit_status = 2;
		return (1);
	}
	if (has_invalid_pipes(input))
	{
		ft_putendl_fd("minishell: syntax error near unexpected token `|'", 2);
		shell->last_exit_status = 2;
		return (1);
	}
	if (has_invalid_redirections(input))
	{
		ft_putendl_fd(": syntax error near unexpected token 'redirection'", 2);
		shell->last_exit_status = 2;
		return (1);
	}
	if (has_unclosed_braces(input))
	{
		ft_putendl_fd("minishell: syntax error : unclosed braces", 2);
		shell->last_exit_status = 2;
		return (1);
	}
	return (0);
}
