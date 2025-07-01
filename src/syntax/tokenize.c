/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 17:22:42 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/01 12:53:56 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Ignore les espaces et tabulations à partir d'un index donné.
 *
 * Avance l'index jusqu'au premier caractère non espace ou tabulation.
 *
 * @param input La chaîne d'entrée
 * @param i Pointeur vers l'index à avancer
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
 *
 * @param c Le caractère à tester	if (!ft_strncmp(str, "<", 1))
 * @return `true` si c'est un séparateur (espace, tab, symbole shell)
 */
bool is_token_delim(char c)
{
	return (c == ' ' || c == '\t' || c == '<' || c == '>' || c == '|' || c == '\0');
}

/**
 * @brief Lit un token brut : soit un mot, soit une séquence entre quotes
 *
 * Si on est sur une quote, appelle parse_quoted_token.
 * Sinon lit jusqu'à la prochaine quote ou un séparateur.
 *
 * @param input Ligne d’entrée
 * @param i Pointeur vers l’index courant (sera avancé)
 * @return char* Le fragment alloué, avec quotes si présentes
 */
/**
 * @brief Lit un token complet qui peut être composé de plusieurs segments
 *
 * Concatène tous les segments adjacents (quotes et mots) en un seul token.
 * Par exemple: ""''echo devient "echo", "hello"world devient "helloworld"
 *
 * @param input Ligne d'entrée
 * @param i Pointeur vers l'index courant (sera avancé)
 * @return char* Le token complet alloué, ou NULL en cas d'erreur
 */
static char *read_complete_token(char *input, int *i)
{
	char *result = NULL;
	char *segment;
	char *temp;
	int start;

	// Concatène tous les segments adjacents (quotes et mots)
	while (input[*i] && !is_token_delim(input[*i]))
	{
		if (input[*i] == '\'' || input[*i] == '"')
			segment = parse_quoted_token(input, i);
		else
		{
			// Lit un mot jusqu'à la prochaine quote ou délimiteur
			start = *i;
			while (input[*i] && input[*i] != '\'' && input[*i] != '"' && !is_token_delim(input[*i]))
				(*i)++;
			segment = ft_substr(input, start, *i - start);
		}
		if (!segment)
		{
			free(result);
			return (NULL);
		}
		// Concatène le segment au résultat
		if (!result)
			result = segment;
		else
		{
			temp = ft_strjoin(result, segment);
			free(result);
			free(segment);
			if (!temp)
				return (NULL);
			result = temp;
		}
	}
	return (result);
}

/**
 * @brief Lit un opérateur spécial (>> / <<)
 *
 * @param input Ligne d’entrée
 * @param i Index à avancer
 * @return char* Opérateur alloué
 */
static char *read_operator(char *input, int *i)
{
	char *op;

	if ((input[*i] == '<' && input[*i + 1] == '<') ||
		(input[*i] == '>' && input[*i + 1] == '>'))
	{
		// Si on a un opérateur de 2 caractères, on l'extrait dans op
		op = ft_substr(input, *i, 2);
		if (!op)
			return (NULL);
		*i += 2;
	}
	else
	{
		// Sinon on extrait un seul caractère
		op = ft_substr(input, *i, 1);
		if (!op)
			return (NULL);
		(*i)++;
	}
	// On retourne l'opérateur extrait
	return (op);
}

/**
 * @brief Concatène une redirection et le fichier cible en un seul token
 *
 * Exemple : si input = "> out.txt", retourne ">out.txt"
 *
 * @param input Ligne d’entrée utilisateur
 * @param i Pointeur vers l’index courant (sera avancé)
 * @return char* Contenu combiné (redir + file), ou NULL si erreur
 */
static char *read_redir_and_file(char *input, int *i)
{
	char *op;
	char *file;
	char *combined;

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

/**
 * @brief Lit un token complet (quote, mot ou opérateur) à partir de l’index donné
 *
 * Crée systématiquement un nouveau token dès qu’un bloc (mot, quote ou opérateur) est détecté.
 *
 * @param input Ligne utilisateur
 * @param i Pointeur vers l’index actuel (sera mis à jour)
 * @return t_token* Un token alloué, ou NULL en cas d’erreur
 */
static t_token *get_next_token(char *input, int *i)
{
	char *content;
	t_token_type type;

	if ((input[*i] == '<' || input[*i] == '>') && input[*i] != '\0')
		content = read_redir_and_file(input, i);
	// Si on tombe sur une quote ou que un caractère non délimité (charactère alphanumérique, etc.),
	// on lit un mot ou une séquence entre quotes.
	else if (!is_token_delim(input[*i]) && input[*i])
		content = read_complete_token(input, i);
	// Sinon, on lit un opérateur spécial (|, >>, &&, etc.)
	else
		content = read_operator(input, i);
	// Si on a atteint la fin de la ligne ou si le contenu est vide, on retourne NULL
	if (!content)
		return (NULL);
	// On détermine le type du token
	type = get_token_type(content);
	// On retourne un nouveau token avec le contenu et son type
	return (token_new(content, type));
}

/**
 * @brief Fonction principale de découpage : transforme la ligne en une liste de tokens
 *
 * @param input Chaîne entrée par l’utilisateur
 * @return t_token* Liste chaînée des tokens
 */
t_token *tokenize(char *input)
{
	t_token *head;
	t_token *last;
	t_token *new;
	int i;

	head = NULL;
	last = NULL;
	i = 0;
	// Ignore les espaces initiaux
	skip_spaces(input, &i);
	while (input[i])
	{
		// On récupère le prochain token à partir de l'index i
		new = get_next_token(input, &i);
		if (!new && input[i])
		{
			// Si malloc fail ou erreur de parsing on free la liste et retourne NULL
			ft_putendl_fd("minishell: error: failed to tokenize input", 2);
			free_token_list(head);
			return (NULL);
		}
		if (new)
			append_token(&head, &last, new);
		// Ignore les espaces entre tokens et en fin d'input
		skip_spaces(input, &i);
	}
	refine_token_types(head);
	// Validation syntaxique de la séquence de tokens
	if (validate_token_sequence(head))
	{
		free_token_list(head);
		return (NULL);
	}
	return (head);
}
