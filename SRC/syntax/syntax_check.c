/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 14:01:02 by hugoganet         #+#    #+#             */
/*   Updated: 2025/05/23 14:12:29 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Vérifie si une ligne est vide (ne contient que des espaces/tabs).
 *
 * @param input La ligne entrée par l’utilisateur.
 * @return int 1 si vide, 0 sinon.
 */
int is_line_empty(char *input)
{
	int i;

	if (!input)
		return (1);
	i = 0;
	while (input[i]) // Parcourt la ligne jusqu'à la fin
	{
		if (input[i] != ' ' && input[i] != '\t') // Vérifie si le caractère n'est pas un espace ou une tabulation
			return (0); // Si un caractère différent est trouvé, la ligne n'est pas vide, return 0
		i++;
	}
	return (1);
}

/**
 * @brief Vérifie si l’entrée contient une quote non fermée.
 *
 * @param input La ligne entrée par l’utilisateur.
 * @return int 1 si une quote est non fermée, 0 sinon.
 */
int has_unclosed_quotes(char *input)
{
	int i;
	char current_quote;

	i = 0;
	current_quote = 0; // Initialise le caractère de quote actuel à 0 (aucune quote ouverte)
	while (input[i])
	{
		if (!current_quote && (input[i] == '\'' || input[i] == '"')) // Si aucune quote n'est ouverte et que le caractère est une quote
			current_quote = input[i]; // Set la quote actuelle
		else if (current_quote && input[i] == current_quote) // Si une quote est ouverte et que le caractère est la même quote
			current_quote = 0; // Ferme la quote
		i++;
	}
	return (current_quote != 0); // Si current_quote n'est pas 0, cela signifie qu'il y a une quote non fermée
}