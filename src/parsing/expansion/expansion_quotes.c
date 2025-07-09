/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_quotes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 16:06:50 by elaudrez          #+#    #+#             */
/*   Updated: 2025/07/09 13:36:03 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

/**
 * @brief Met à jour l'état des quotes (simples/doubles) en fonction du
 *        caractère courant, spécifiquement pour le module d'expansion.
 *        Réutilise la logique du module quotes pour respecter le DRY.
 *
 * @param state L'état actuel de l'expansion.
 */
void	expansion_update_quote_state(t_expansion_state *state)
{
	char	c;

	c = state->input[state->i];
	// Si c est une quote simple et que l'on n'est pas dans des doubles quotes,
	// on inverse l'état des quotes simples.
	if (c == '\'' && !state->in_double_quotes)
		state->in_single_quotes = !state->in_single_quotes;
	// Si c est une quote double et que l'on n'est pas dans des simples quotes,
	// on inverse l'état des quotes doubles.
	else if (c == '"' && !state->in_single_quotes)
		state->in_double_quotes = !state->in_double_quotes;
}
