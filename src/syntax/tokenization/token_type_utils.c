/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_type_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 00:00:00 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/07 15:00:49 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "syntax.h"

/**
 * @brief Associe une chaîne à son type de token.
 *
 * Analyse la chaîne donnée et détermine le type de token correspondant.
 * Reconnaît les opérateurs de redirection, les pipes et les mots.
 *
 * @param str Token brut
 * @return t_token_type Type du token
 */
t_token_type	get_token_type(char *str)
{
	if (!str)
		return (WORD);
	if (!ft_strncmp(str, "<<", 2))
		return (HEREDOC);
	if (!ft_strncmp(str, ">>", 2))
		return (REDIR_APPEND);
	if (!ft_strncmp(str, "<", 1))
		return (REDIR_INPUT);
	if (!ft_strncmp(str, ">", 1))
		return (REDIR_OUTPUT);
	if (!ft_strncmp(str, "|", 2))
		return (PIPE);
	return (WORD);
}

/**
 * @brief Vérifie si le type de token correspond à une redirection.
 *
 * Teste si le type donné est l'un des quatre types de redirection
 * supportés par le shell.
 *
 * @param type Le type du token à vérifier
 * @return true si c'est une redirection (<, >, <<, >>), false sinon
 */
bool	is_redirection(t_token_type type)
{
	return (type == REDIR_INPUT || type == REDIR_OUTPUT
		|| type == REDIR_APPEND || type == HEREDOC);
}

/**
 * @brief Vérifie si le type est un opérateur logique (PIPE).
 *
 * Détermine si le type de token représente un opérateur logique
 * qui connecte deux commandes.
 *
 * @param type Le type du token à vérifier
 * @return true si c'est un opérateur logique, false sinon
 */
bool	is_logical_operator(t_token_type type)
{
	return (type == PIPE);
}
