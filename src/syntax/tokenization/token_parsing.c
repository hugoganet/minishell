/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_parsing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 00:00:00 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/04 09:41:59 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "syntax.h"

/**
 * @brief Ignore les espaces et tabulations à partir d'un index donné
 *
 * Avance l'index jusqu'au premier caractère non espace ou non tabulation.
 * Cette fonction protège contre les accès mémoire invalides en vérifiant
 * la validité des paramètres et l'existence du caractère courant.
 *
 * @param input La chaîne d'entrée à analyser (doit être non-NULL)
 * @param i Pointeur vers l'index à avancer (doit être non-NULL)
 */
void skip_spaces(char *input, int *i)
{
	if (!input || !i)
		return;
	while (input[*i] && (input[*i] == ' ' || input[*i] == '\t'))
		(*i)++;
}

/**
 * @brief Vérifie si le caractère courant est un séparateur de token
 *
 * Utilisé pour détecter les limites des mots, quotes ou opérateurs.
 * Un délimiteur marque la fin d'un token et le début d'un nouveau.
 *
 * Les délimiteurs reconnus sont :
 * - Espaces et tabulations (séparent les arguments)
 * - Opérateurs de redirection (<, >)
 * - Pipe (|)
 * - Fin de chaîne (\0)
 *
 * @param c Le caractère à tester
 * @return true si c'est un séparateur (espace, tab, symbole shell), false sinon
 */
bool is_token_delim(char c)
{
	return (c == ' ' || c == '\t' || c == '<' || c == '>' || c == '|' || c == '\0');
}

/**
 * @brief Lit un segment de texte jusqu'à la prochaine quote ou délimiteur
 *
 * Cette fonction auxiliaire extrait un mot simple (sans quotes) en s'arrêtant
 * dès qu'elle rencontre une quote ou un délimiteur de token.
 *
 * @param input La chaîne d'entrée à analyser
 * @param i Pointeur vers l'index courant (sera avancé)
 * @return char* Segment alloué ou NULL en cas d'erreur
 */
char *read_word_segment(char *input, int *i)
{
	int start;

	start = *i;
	while (input[*i] && input[*i] != '\'' && input[*i] != '"' && !is_token_delim(input[*i]))
		(*i)++;
	return (ft_substr(input, start, *i - start));
}

/**
 * @brief Concatène deux chaînes en libérant les anciennes
 *
 * Fonction utilitaire pour la concaténation sécurisée avec gestion mémoire.
 * Libère automatiquement les deux chaînes d'origine après concaténation.
 *
 * @param result Chaîne de résultat (sera libérée)
 * @param segment Nouveau segment à ajouter (sera libéré)
 * @return char* Nouvelle chaîne concaténée ou NULL en cas d'erreur
 */
char *join_and_free(char *result, char *segment)
{
	char *temp;

	if (!result)
		return (segment);
	temp = ft_strjoin(result, segment);
	free(result);
	free(segment);
	return (temp);
}
