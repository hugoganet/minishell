/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   braces_validation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 00:00:00 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/07 15:04:14 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "syntax.h"

/**
 * @brief Gère la fermeture d'une accolade.
 *
 * Vérifie qu'il y a une accolade ouverte correspondante avant de
 * décrémenter le compteur. Retourne une erreur si on ferme une
 * accolade sans qu'il y en ait une d'ouverte.
 *
 * @param count Pointeur vers le compteur d'accolades
 * @return int 1 si erreur (fermeture sans ouverture), 0 si OK
 */
static int	handle_closing_brace(int *count)
{
	if (*count == 0)
		return (1);
	(*count)--;
	return (0);
}

/**
 * @brief Vérifie que les accolades sont équilibrées.
 *
 * Parcourt la chaîne en ignorant les accolades dans les quotes.
 * Utilise un compteur pour vérifier l'équilibrage : incrémente
 * pour chaque '{' et décrémente pour chaque '}'. Retourne une
 * erreur si le compteur final n'est pas zéro.
 *
 * @param input Ligne de commande
 * @return int 1 si erreur (accolades non fermées), 0 si OK
 */
int	has_unclosed_braces(char *input)
{
	int		i;
	int		count;
	char	quote_state;

	i = 0;
	count = 0;
	quote_state = 0;
	while (input[i])
	{
		update_quote_state(&quote_state, input[i]);
		if (!quote_state && input[i] == '{')
			count++;
		else if (!quote_state && input[i] == '}')
		{
			if (handle_closing_brace(&count))
				return (1);
		}
		i++;
	}
	return (count != 0);
}
