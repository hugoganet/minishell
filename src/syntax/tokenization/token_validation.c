/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_validation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 00:00:00 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/07 15:02:13 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "syntax.h"

/**
 * @brief Extrait une sous-chaîne entre quotes simples ou doubles.
 *
 * Parse un token quoté en extrayant le contenu entre les quotes.
 * Gère les quotes simples et doubles, en préservant les quotes
 * dans le token retourné.
 *
 * @param input Chaîne d'entrée
 * @param i Pointeur vers l'index courant (pointant sur la quote ouvrante)
 * @return char* Sous-chaîne allouée (avec les quotes), ou NULL en cas d'erreur
 */
char	*parse_quoted_token(char *input, int *i)
{
	char	quote;
	int		start;
	int		end;
	char	*token_new;

	quote = input[*i];
	start = *i;
	end = start + 1;
	while (input[end] && input[end] != quote)
		end++;
	if (input[end] != quote)
	{
		*i = end;
		return (ft_substr(input, start, end - start));
	}
	*i = end + 1;
	token_new = ft_substr(input, start, (size_t)end - start + 1);
	if (!token_new)
		return (NULL);
	return (token_new);
}

/**
 * @brief Affiche une erreur de syntaxe avec un token donné.
 *
 * Formate et affiche un message d'erreur de syntaxe standard
 * sur la sortie d'erreur, incluant le token fautif.
 *
 * @param token Le token fautif
 * @return int Toujours 1 (pour être utilisé directement dans un return)
 */
int	print_syntax_error(char *token)
{
	ft_putstr_fd("minishell: syntax error near unexpected token '",
		STDERR_FILENO);
	ft_putstr_fd(token, STDERR_FILENO);
	ft_putendl_fd("'", STDERR_FILENO);
	return (1);
}

/**
 * @brief Vérifie la validité syntaxique de la séquence de tokens.
 *
 * Cette fonction détecte les erreurs suivantes :
 * - Opérateur logique au début ou à la fin de la ligne
 * - Opérateurs logiques consécutifs (ex: `| |`)
 *
 * Si une erreur est détectée, elle est affichée sur stderr.
 *
 * @param head Pointeur vers le premier token de la liste
 * @return int 0 si la séquence est valide, 1 sinon
 */
int	validate_token_sequence(t_token *head)
{
	t_token	*prev;
	t_token	*curr;

	prev = NULL;
	curr = head;
	while (curr)
	{
		if ((is_logical_operator(curr->type) && !prev)
			|| (prev && is_logical_operator(prev->type)
				&& is_logical_operator(curr->type)))
			return (print_syntax_error(curr->str));
		prev = curr;
		curr = curr->next;
	}
	if (prev && is_logical_operator(prev->type))
		return (print_syntax_error(prev->str));
	return (0);
}
