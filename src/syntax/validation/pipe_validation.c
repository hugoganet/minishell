/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_validation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 00:00:00 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/07 15:05:02 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "syntax.h"

/**
 * @brief Vérifie si les pipes sont mal placés : en début ou en fin.
 *
 * Parcourt la chaîne d'entrée en ignorant les pipes dans les quotes.
 * Vérifie qu'aucun pipe ne se trouve en début de ligne (index 0) ou
 * en fin de ligne (suivi du caractère de fin de chaîne).
 *
 * @param input Ligne entrée par l'utilisateur
 * @return int 1 si erreur, 0 si syntaxe correcte
 */
int	has_invalid_pipes(char *input)
{
	int		i;
	char	quote_state;

	i = 0;
	quote_state = 0;
	while (input[i])
	{
		update_quote_state(&quote_state, input[i]);
		if (!quote_state && input[i] == '|')
		{
			if (i == 0 || input[i + 1] == '\0')
				return (1);
		}
		i++;
	}
	return (0);
}
