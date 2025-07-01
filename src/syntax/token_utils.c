/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 17:26:12 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/01 12:45:36 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Associe une chaîne à son type de token.
 *
 * @param str Token brut
 * @return t_token_type Type du token
 */
t_token_type get_token_type(char *str)
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
 * @param type Le type du token à vérifier
 * @return true si c'est une redirection (<, >, <<, >>), false sinon
 */
bool is_redirection(t_token_type type)
{
	return (type == REDIR_INPUT || type == REDIR_OUTPUT || type == REDIR_APPEND || type == HEREDOC);
}

/**
 * @brief Extrait une sous-chaîne entre quotes simples ou doubles.
 *
 * @param input Chaîne d'entrée
 * @param i Pointeur vers l'index courant (pointant sur la quote ouvrante)
 * @return char* Sous-chaîne allouée (sans les quotes), ou NULL en cas d'erreur
 */
char *parse_quoted_token(char *input, int *i)
{
	char quote;
	int start;
	int end;
	char *token_new;

	quote = input[*i];
	start = *i;
	// On commence à parcourir input juste après la quote ouvrante
	end = start + 1;
	// Avance l'index jusqu'à la quote fermante correspondante
	while (input[end] && input[end] != quote)
		end++;
	// Si on n'a pas trouvé de quote fermante, erreur
	if (input[end] != quote)
	{
		// Quote non fermée - on pourrait retourner une erreur
		*i = end;
		return (ft_substr(input, start, end - start));
	}
	// On set l'index à la fin de la quote fermante
	*i = end + 1;
	// On extrait la sous-chaîne avec les quotes
	token_new = ft_substr(input, start, (size_t)end - start + 1);
	if (!token_new)
		return (NULL);
	return (token_new);
}

/**
 * @brief Vérifie si le type est un opérateur logique (PIPE).
 *
 * @param type Le type du token à vérifier
 * @return true si c'est un opérateur logique, false sinon
 */
static bool is_logical_operator(t_token_type type)
{
	return (type == PIPE);
}

/**
 * @brief Affiche une erreur de syntaxe avec un token donné.
 *
 * @param token Le token fautif
 * @return int Toujours 1 (pour être utilisé directement dans un return)
 */
int print_syntax_error(char *token)
{
	ft_putstr_fd("minishell: syntax error near unexpected token '", 2);
	ft_putstr_fd(token, 2);
	ft_putendl_fd("'", 2);
	return (1);
}

/**
 * @brief Vérifie la validité syntaxique de la séquence de tokens.
 *
 * Cette fonction détecte les erreurs suivantes :
 *
 * - opérateur logique au début ou à la fin de la ligne
 *
 * - opérateurs logiques consécutifs (ex: `| |`)
 *
 * Si une erreur est détectée, elle est affichée sur stderr,
 * et la fonction retourne 1.
 *
 * @param head Pointeur vers le premier token de la liste
 * @return `int` 0 si la séquence est valide, 1 sinon
 */
int validate_token_sequence(t_token *head)
{
	t_token *prev;
	t_token *curr;

	prev = NULL;
	curr = head;
	while (curr)
	{
		if ((is_logical_operator(curr->type) && !prev) || (prev && is_logical_operator(prev->type) && is_logical_operator(curr->type)))
			return (print_syntax_error(curr->str));
		prev = curr;
		curr = curr->next;
	}
	if (prev && is_logical_operator(prev->type))
		return (print_syntax_error(prev->str));
	return (0);
}
