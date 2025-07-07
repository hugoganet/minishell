/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 15:14:49 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/07 16:03:52 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "syntax.h"

/**
 * @brief Met à jour l'état d'ouverture/fermeture de quote en
 * fonction du caractère courant.
 *
 * Gère l'état des quotes lors du parsing. Si aucune quote n'est ouverte
 * et qu'on rencontre une quote, elle devient active. Si une quote est
 * déjà active et qu'on rencontre la même, elle se ferme.
 *
 * @param quote_state Pointeur vers la quote courante (' ou "), ou 0 si aucune.
 * @param c Caractère à analyser.
 */
void	update_quote_state(char *quote_state, char c)
{
	if (!*quote_state && (c == '\'' || c == '"'))
		*quote_state = c;
	else if (*quote_state && c == *quote_state)
		*quote_state = 0;
}

/**
 * @brief Vérifie si une parenthèse est vide : () ou (     ).
 *
 * Analyse le contenu entre une parenthèse ouvrante et sa fermeture
 * pour détecter si elle ne contient que des espaces ou est vide.
 *
 * @param input Chaîne d'entrée
 * @param i Position de la parenthèse ouvrante
 * @return int 1 si vide, 0 sinon
 */
int	is_parenthesis_empty(char *input, int i)
{
	i++;
	while (input[i] == ' ' || input[i] == '\t')
		i++;
	return (input[i] == ')');
}
