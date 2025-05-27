/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 15:14:49 by hugoganet         #+#    #+#             */
/*   Updated: 2025/05/23 16:17:34 by hugoganet        ###   ########.fr       */
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
	if (!*quote_state && (c == '\'' || c == '"')) // Si aucune quote ouverte
		*quote_state = c; // Ouvre une quote
	else if (*quote_state && c == *quote_state) // Si une quote est ouverte et que le caractère courant est la même quote
		*quote_state = 0; // Ferme la quote
}

/**
 * @brief Vérifie si une parenthèse est vide : () ou (     ).
 *
 * @param input Chaîne d'entrée
 * @param i Position de la parenthèse ouvrante
 * @return int 1 si vide, 0 sinon
 */
int is_parenthesis_empty(char *input, int i)
{
	i++; // On passe le caractère '('
	while (input[i] == ' ' || input[i] == '\t') // Ignore les espaces et tabulations
		i++; // On passe les espaces
	return (input[i] == ')'); // Si le caractère suivant est ')', la parenthèse est vide
}