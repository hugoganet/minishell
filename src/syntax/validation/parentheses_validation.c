/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parentheses_validation.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 00:00:00 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/04 09:41:59 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "syntax.h"

/**
 * @brief Gère l'ouverture d'une parenthèse et vérifie qu'elle n'est pas vide.
 *
 * Incrémente le compteur de parenthèses ouvertes et vérifie que la
 * parenthèse n'est pas immédiatement fermée (parenthèses vides).
 *
 * @param input La chaîne d'entrée
 * @param i L'index de la parenthèse ouvrante
 * @param count Pointeur vers le compteur de parenthèses
 * @return int 1 si erreur (parenthèses vides), 0 si OK
 */
static int handle_opening_parenthesis(char *input, int i, int *count)
{
	(*count)++;
	if (is_parenthesis_empty(input, i))
		return (1);
	return (0);
}

/**
 * @brief Gère la fermeture d'une parenthèse.
 *
 * Vérifie qu'il y a une parenthèse ouverte correspondante avant de
 * décrémenter le compteur. Retourne une erreur si on ferme une
 * parenthèse sans qu'il y en ait une d'ouverte.
 *
 * @param count Pointeur vers le compteur de parenthèses
 * @return int 1 si erreur (fermeture sans ouverture), 0 si OK
 */
static int handle_closing_parenthesis(int *count)
{
	if (*count == 0)
		return (1);
	(*count)--;
	return (0);
}

/**
 * @brief Vérifie que les parenthèses sont équilibrées et non vides.
 *
 * Parcourt la chaîne en ignorant les parenthèses dans les quotes.
 * Utilise un compteur pour vérifier l'équilibrage et appelle les
 * fonctions helper pour valider chaque parenthèse.
 *
 * @param input Ligne de commande
 * @return int 1 si erreur, 0 si OK
 */
int has_unmatched_parentheses(char *input)
{
	int i;
	int count;
	char quote;

	i = 0;
	count = 0;
	quote = 0;
	while (input[i])
	{
		update_quote_state(&quote, input[i]);
		if (!quote && input[i] == '(')
		{
			if (handle_opening_parenthesis(input, i, &count))
				return (1);
		}
		else if (!quote && input[i] == ')')
		{
			if (handle_closing_parenthesis(&count))
				return (1);
		}
		i++;
	}
	return (count != 0);
}
