/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_quotes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 16:06:50 by elaudrez          #+#    #+#             */
/*   Updated: 2025/07/09 18:08:55 by elaudrez         ###   ########.fr       */
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
	if (c == '\'' && !state->in_double_quotes)
		state->in_single_quotes = !state->in_single_quotes;
	else if (c == '"' && !state->in_single_quotes)
		state->in_double_quotes = !state->in_double_quotes;
}
