/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 14:01:02 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/02 16:11:59 by hugoganet        ###   ########.fr       */
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
	while (input[i])
	{
		if (input[i] != ' ' && input[i] != '\t')
			return (0);
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
	char quote_state;

	i = 0;
	quote_state = 0;
	while (input[i])
	{
		update_quote_state(&quote_state, input[i]);
		i++;
	}
	return (quote_state != 0);
}

/**
 * @brief Vérifie si les pipes sont mal placés : en début ou en fin.
 *
 * @param input Ligne entrée par l’utilisateur
 * @return int 1 si erreur, 0 si syntaxe correcte
 */
int has_invalid_pipes(char *input)
{
	int i;
	char quote_state;

	i = 0;
	quote_state = 0;
	while (input[i])
	{
		update_quote_state(&quote_state, input[i]);
		if (!quote_state && input[i] == '|')
		{
			if (i == 0 || input[i + 1] == '\0')
				return (1);
		}
		i++;
	}
	return (0);
}

/**
 * @brief Vérifie si les redirections sont mal placées ou incomplètes.
 *
 * @param input La ligne utilisateur
 * @return int 1 si erreur, 0 si syntaxe correcte
 */
static int get_redirection_length(char *input, int i)
{
	if (input[i + 1] == input[i])
		return (2);
	return (1);
}

static int skip_whitespace(char *input, int i)
{
	while (input[i] == ' ' || input[i] == '\t')
		i++;
	return (i);
}

static int is_invalid_after_redir(char *input, int i)
{
	return (input[i] == '\0' || input[i] == '|' || input[i] == '<' || input[i] == '>');
}

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

/**
 * @brief Vérifie que les parenthèses sont équilibrées et non vides.
 *
 * @param input Ligne de commande
 * @return int 1 si erreur, 0 si OK
 */
static int handle_opening_parenthesis(char *input, int i, int *count)
{
	(*count)++;
	if (is_parenthesis_empty(input, i))
		return (1);
	return (0);
}

static int handle_closing_parenthesis(int *count)
{
	if (*count == 0)
		return (1);
	(*count)--;
	return (0);
}

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

/**
 * @brief Vérifie que les accolades sont équilibrées.
 *
 * @param input Ligne de commande
 * @return `int` 1 si erreur (accolades non fermées), 0 si OK
 */
static int handle_closing_brace(int *count)
{
	if (*count == 0)
		return (1);
	(*count)--;
	return (0);
}

int has_unclosed_braces(char *input)
{
	int i;
	int count;
	char quote_state;

	i = 0;
	count = 0;
	quote_state = 0;
	while (input[i])
	{
		update_quote_state(&quote_state, input[i]);
		if (!quote_state && input[i] == '{')
			count++;
		else if (!quote_state && input[i] == '}')
		{
			if (handle_closing_brace(&count))
				return (1);
		}
		i++;
	}
	return (count != 0);
}
