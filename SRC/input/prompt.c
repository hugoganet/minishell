/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 13:01:41 by hugoganet         #+#    #+#             */
/*   Updated: 2025/05/23 13:05:38 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Affiche le prompt et lit l’entrée utilisateur.
 *        Si la ligne n’est pas vide, elle est ajoutée à l’historique.
 *
 * @return char* La ligne lue (allouée dynamiquement). NULL si Ctrl+D.
 */
char *prompt_readline(void)
{
	char *input;

	input = readline("minishell> ");
	if (input && *input)
		add_history(input);
	return (input);
}