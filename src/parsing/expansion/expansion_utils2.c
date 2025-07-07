/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 16:11:28 by elaudrez          #+#    #+#             */
/*   Updated: 2025/07/07 16:19:04 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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