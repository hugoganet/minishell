/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_validation.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 00:00:00 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/04 09:41:59 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "syntax.h"

/**
 * @brief Détermine la longueur d'un opérateur de redirection.
 *
 * Vérifie si l'opérateur de redirection est simple (< ou >) ou double
 * (<< pour heredoc ou >> pour append). Retourne la longueur appropriée.
 *
 * @param input La chaîne d'entrée
 * @param i L'index du premier caractère de redirection
 * @return int 2 si double redirection, 1 si simple
 */
static int get_redirection_length(char *input, int i)
{
	if (input[i + 1] == input[i])
		return (2);
	return (1);
}

/**
 * @brief Ignore les espaces et tabulations à partir d'un index.
 *
 * Avance l'index jusqu'au premier caractère non-espace et non-tabulation.
 * Utilisé pour vérifier ce qui suit une redirection après les espaces.
 *
 * @param input La chaîne d'entrée
 * @param i L'index de départ
 * @return int Le nouvel index après les espaces
 */
static int skip_whitespace(char *input, int i)
{
	while (input[i] == ' ' || input[i] == '\t')
		i++;
	return (i);
}

/**
 * @brief Vérifie si le caractère après une redirection est invalide.
 *
 * Une redirection doit être suivie d'un nom de fichier valide, pas
 * d'un autre opérateur ou de la fin de ligne.
 *
 * @param input La chaîne d'entrée
 * @param i L'index à vérifier
 * @return int 1 si invalide, 0 si valide
 */
static int is_invalid_after_redir(char *input, int i)
{
	return (input[i] == '\0' || input[i] == '|' || input[i] == '<' || input[i] == '>');
}

/**
 * @brief Vérifie si les redirections sont mal placées ou incomplètes.
 *
 * Parcourt la chaîne en ignorant les redirections dans les quotes.
 * Pour chaque redirection trouvée, vérifie qu'elle est suivie d'un
 * nom de fichier valide (pas d'un autre opérateur ou de la fin de ligne).
 *
 * @param input La ligne utilisateur
 * @return int 1 si erreur, 0 si syntaxe correcte
 */
int has_invalid_redirections(char *input)
{
	int i;
	char quote_state;
	int redir_len;

	i = 0;
	quote_state = 0;
	while (input[i])
	{
		update_quote_state(&quote_state, input[i]);
		if (!quote_state && (input[i] == '<' || input[i] == '>'))
		{
			redir_len = get_redirection_length(input, i);
			i += redir_len;
			i = skip_whitespace(input, i);
			if (is_invalid_after_redir(input, i))
				return (1);
			continue;
		}
		i++;
	}
	return (0);
}
