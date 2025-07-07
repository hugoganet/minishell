/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_reading.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 00:00:00 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/07 15:00:27 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "syntax.h"

/**
 * @brief Lit un segment (quote ou mot) à partir de la position courante
 *
 * Détermine le type de segment à lire et appelle la fonction appropriée.
 * Gère à la fois les segments entre quotes et les mots simples.
 *
 * @param input La chaîne d'entrée à analyser
 * @param i Pointeur vers l'index courant (sera avancé)
 * @return char* Segment lu ou NULL en cas d'erreur
 */
char	*read_next_segment(char *input, int *i)
{
	if (input[*i] == '\'' || input[*i] == '"')
		return (parse_quoted_token(input, i));
	else
		return (read_word_segment(input, i));
}

/**
 * @brief Lit un token complet composé de plusieurs segments adjacents
 *
 * Cette fonction concatène tous les segments adjacents (quotes vides, quotes
 * pleines, et mots) en un seul token. Elle respecte le comportement bash où
 * des éléments comme ""''echo ou "hello"world sont traités comme un seul mot.
 *
 * Exemples de concaténation :
 * - ""''echo → "echo" (quotes vides + mot)
 * - "hello"world → "helloworld" (quote + mot)
 * - 'a'"b"c → "abc" (multiple quotes + mot)
 *
 * @param input La chaîne d'entrée utilisateur
 * @param i Pointeur vers l'index courant (sera mis à jour au fur et à mesure)
 * @return char* Token complet alloué, ou NULL en cas d'erreur mémoire
 */
char	*read_complete_token(char *input, int *i)
{
	char	*result;
	char	*segment;

	result = NULL;
	while (input[*i] && !is_token_delim(input[*i]))
	{
		segment = read_next_segment(input, i);
		if (!segment)
		{
			free(result);
			return (NULL);
		}
		result = join_and_free(result, segment);
		if (!result)
			return (NULL);
	}
	return (result);
}

/**
 * @brief Lit un opérateur spécial (>> / <<)
 *
 * Détermine s'il s'agit d'un opérateur simple (< > |) ou double (<< >>)
 * et extrait la chaîne correspondante.
 *
 * @param input Ligne d'entrée
 * @param i Index à avancer
 * @return char* Opérateur alloué
 */
char	*read_operator(char *input, int *i)
{
	char	*op;

	if ((input[*i] == '<' && input[*i + 1] == '<')
		|| (input[*i] == '>' && input[*i + 1] == '>'))
	{
		op = ft_substr(input, *i, 2);
		if (!op)
			return (NULL);
		*i += 2;
	}
	else
	{
		op = ft_substr(input, *i, 1);
		if (!op)
			return (NULL);
		(*i)++;
	}
	return (op);
}

/**
 * @brief Concatène une redirection et le fichier cible en un seul token
 *
 * Exemple : si input = "> out.txt", retourne ">out.txt"
 *
 * @param input Ligne d'entrée utilisateur
 * @param i Pointeur vers l'index courant (sera avancé)
 * @return char* Contenu combiné (redir + file), ou NULL si erreur
 */
char	*read_redir_and_file(char *input, int *i)
{
	char	*op;
	char	*file;
	char	*combined;

	op = read_operator(input, i);
	if (!op)
		return (NULL);
	skip_spaces(input, i);
	file = read_complete_token(input, i);
	if (!file)
	{
		free(op);
		return (NULL);
	}
	combined = ft_strjoin(op, file);
	free(op);
	free(file);
	return (combined);
}
