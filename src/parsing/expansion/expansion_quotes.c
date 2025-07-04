#include "expansion.h"

/**
 * @brief Met à jour l'état des quotes (simples/doubles) en fonction du
 *        caractère courant, spécifiquement pour le module d'expansion.
 *        Réutilise la logique du module quotes pour respecter le DRY.
 *
 * @param state L'état actuel de l'expansion.
 */
void expansion_update_quote_state(t_expansion_state *state)
{
	char c = state->input[state->i];

	if (c == '\'' && !state->in_double_quotes)
		state->in_single_quotes = !state->in_single_quotes;
	else if (c == '"' && !state->in_single_quotes)
		state->in_double_quotes = !state->in_double_quotes;
}
