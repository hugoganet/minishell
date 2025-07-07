/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_specials.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 16:07:27 by elaudrez          #+#    #+#             */
/*   Updated: 2025/07/07 21:01:39 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"
#include "libft.h"

/**
 * @brief Gère l'expansion d'une variable spéciale (ex: $?, $0).
 *        Ne traite QUE les cas vraiment spéciaux.
 *
 * @param state L'état actuel de l'expansion.
 * @param last_exit_status Le code de retour de la dernière commande.
 * @return Le contenu de la variable spéciale, ou NULL si non applicable.
 */
char	*expand_special_variable(t_expansion_state *state, int last_exit_status)
{
	char	next_char;

	if (state->input[state->i + 1] == '\0')
		return (NULL);
	next_char = state->input[state->i + 1];
	if (next_char == '?')
	{
		state->i += 2;
		return (ft_itoa(last_exit_status));
	}
	if (next_char == '0')
	{
		state->i += 2;
		return (ft_strdup("minishell"));
	}
	return (NULL);
}

/**
 * @brief Essaie d'expander les variables spéciales ou les translated strings.
 *		  Gère les variables spéciales comme $?, $0, $"".
 *
 * @param state L'état actuel de l'expansion.
 * @param last_exit_status Le code de retour de la dernière commande.
 * @return `true` si une variable spéciale a été trouvée et expansée,
 * false sinon.
 */
bool	try_expand_special_vars(t_expansion_state *state, int last_exit_status)
{
	char	*var_value;
	int		end_pos;

	var_value = extract_translated_string(state->input, state->i, &end_pos);
	if (var_value)
	{
		append_and_free(state, var_value);
		state->i = end_pos;
		return (true);
	}
	var_value = expand_special_variable(state, last_exit_status);
	if (var_value)
	{
		append_and_free(state, var_value);
		return (true);
	}
	return (false);
}
