/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 13:01:41 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/06 20:55:04 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Affiche le prompt et lit l'entrée utilisateur.
 * On ne gère pas SIGQUIT ici car il faut l'ignorer dans le processus readline.
 *
 * @return La ligne lue.
 */
char *prompt_readline(void)
{
	char *input;

	if (!isatty(STDIN_FILENO))
		return (get_next_line(STDIN_FILENO));
	input = readline("minishell> ");
	if (input && *input)
		add_history(input);
	return (input);
}