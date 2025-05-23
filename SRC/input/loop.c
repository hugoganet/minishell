/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 13:16:41 by hugoganet         #+#    #+#             */
/*   Updated: 2025/05/23 16:04:48 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Boucle principale du shell : lit les entrées, les traite,
 *        et quitte proprement sur Ctrl+D.
 *
 * @param shell Pointeur vers la structure du shell.
 */
void shell_loop(t_shell *shell)
{
	char *input;

	shell->last_exit_status = 0; // À supprimer après, c'est juste pour -Werror
	while (1)
	{
		input = prompt_readline(); // Affiche le prompt et lit l'entrée utilisateur
		if (!input) // Si l'entrée est NULL (Ctrl+D)
		{
			write(1, "exit\n", 5);
			break;
		}
		if (!is_line_empty(input)) // Si la ligne n'est pas vide
		{
			if (has_unclosed_quotes(input)) // Vérifie si la ligne a une quote non fermée
				ft_putendl_fd("minishell: syntax error: unclosed quote", 2);
			else if (has_invalid_pipes(input)) // Vérifie si la ligne a des pipes mal placés
				ft_putendl_fd("minishell: syntax error near unexpected token `|'", 2);
			else if (has_invalid_redirections(input))
				ft_putendl_fd("minishell: syntax error near unexpected token `>'", 2);
			else if (has_unmatched_parentheses(input))
				ft_putendl_fd("minishell: syntax error: unmatched or empty parentheses", 2);
			else
			{
				// parsing à venir
			}
		}
		free(input);
	}
}
