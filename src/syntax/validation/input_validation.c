/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_validation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 00:00:00 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/04 09:41:59 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "syntax.h"

/**
 * @brief Vérifie si une ligne est vide (ne contient que des espaces/tabs).
 *
 * Parcourt la chaîne d'entrée pour détecter si elle ne contient que des
 * caractères d'espacement (espaces et tabulations). Une ligne NULL est
 * considérée comme vide.
 *
 * @param input La ligne entrée par l'utilisateur.
 * @return int 1 si vide, 0 sinon.
 */
int is_line_empty(char *input)
{
	int i;

	if (!input)
		return (1);
	i = 0;
	while (input[i])
	{
		if (input[i] != ' ' && input[i] != '\t')
			return (0);
		i++;
	}
	return (1);
}

/**
 * @brief Vérifie si l'entrée contient une quote non fermée.
 *
 * Utilise la fonction update_quote_state pour traquer l'état des quotes
 * dans la chaîne d'entrée. Retourne 1 si une quote simple ou double
 * reste ouverte à la fin de la chaîne.
 *
 * @param input La ligne entrée par l'utilisateur.
 * @return int 1 si une quote est non fermée, 0 sinon.
 */
int has_unclosed_quotes(char *input)
{
	int i;
	char quote_state;

	i = 0;
	quote_state = 0;
	while (input[i])
	{
		update_quote_state(&quote_state, input[i]);
		i++;
	}
	return (quote_state != 0);
}
