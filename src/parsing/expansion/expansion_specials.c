/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_specials.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 16:07:27 by elaudrez          #+#    #+#             */
/*   Updated: 2025/07/07 16:08:33 by elaudrez         ###   ########.fr       */
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
