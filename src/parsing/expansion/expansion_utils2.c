/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 16:11:28 by elaudrez          #+#    #+#             */
/*   Updated: 2025/07/09 13:22:59 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Initialise l'état de l'expansion.
 * 		  Alloue de la mémoire pour la chaîne de sortie et
 * 		  initialise les variables d'état.
 *
 * @param state L'état d'expansion à initialiser.
 * @param input La chaîne d'entrée à traiter.
 * @return `1` en cas de succès, `0` en cas d'échec
 */
int	init_expansion_state(t_expansion_state *state, const char *input)
{
	// On initialiase l'état d'expansion à l'input donné.
	state->input = input;
	// On alloue un pointeur pour la chaîne de sortie.
	state->output = ft_strdup("");
	if (!state->output)
		return (0);
	// On initialise l'index de la chaîne d'entrée.
	state->i = 0;
	// ON initialise les autres variables d'état.
	state->in_single_quotes = false;
	state->in_double_quotes = false;
	state->expanded_to_empty = false;
	return (1);
}

/**
 * @brief Vérifie si un caractère peut commencer un nom de variable valide.
 *
 * @param c Le caractère à vérifier.
 * @return true si le caractère peut commencer une variable, false sinon.
 */
bool	is_valid_var_start(char c)
{
	return (ft_isalpha(c) || c == '_');
}

/**
 * @brief Vérifie si un caractère peut faire partie d'un nom de variable.
 *
 * @param c Le caractère à vérifier.
 * @return true si le caractère peut faire partie d'une variable, false sinon.
 */
bool	is_valid_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

/**
 * @brief Vérifie si la séquence après $ est une translated string ($"...").
 *
 * @param input La chaîne d'entrée.
 * @param dollar_pos La position du caractère '$'.
 * @return true si c'est une translated string, false sinon.
 */
bool	is_translated_string(const char *input, int dollar_pos)
{
	return (input[dollar_pos + 1] == '"');
}
