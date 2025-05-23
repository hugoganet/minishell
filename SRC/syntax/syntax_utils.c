/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 15:14:49 by hugoganet         #+#    #+#             */
/*   Updated: 2025/05/23 15:19:58 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @brief Met à jour l’état d’ouverture/fermeture de quote en fonction du caractère courant.
 *
 * @param quote_state Pointeur vers la quote courante (' ou "), ou 0 si aucune.
 * @param c Caractère à analyser.
 */
void update_quote_state(char *quote_state, char c)
{
	if (!*quote_state && (c == '\'' || c == '"'))
		*quote_state = c;
	else if (*quote_state && c == *quote_state)
		*quote_state = 0;
}