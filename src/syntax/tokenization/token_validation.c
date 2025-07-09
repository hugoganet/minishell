/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_validation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 00:00:00 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/09 11:23:49 by hugoganet        ###   ########.fr       */
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

	// Quote devient le caractère courant (une quote simple ou double)
	quote = input[*i];
	// `start` prend l'index de la quote courante
	start = *i;
	// `end` est initialisé à start + 1 (charactère suivant la quote)
	end = start + 1;
	// On avance `end` jusqu'à trouver la quote fermante correspondante
	while (input[end] && input[end] != quote)
		end++;
	// Si on atteint la fin de la chaîne sans trouver la quote fermante,
	if (input[end] != quote)
	{
		// l'index de i est mis à jour pour pointer sur la fin de la chaîne 
		*i = end;
		// et on retourne la chaîne
		return (ft_substr(input, start, end - start));
	}
	// Si on trouve la quote fermante, on avance `end` d'un caractère
	// pour inclure la quote fermante dans le token		
	*i = end + 1;
	// On retourne une nouvelle sous-chaîne qui inclut les quotes
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
	// On parcourt la liste de tokens
	// et on vérifie les conditions d'erreur
	while (curr)
	{
		// Si le token courant est un opérateur logique '|'
		// et qu'il est le premier de la liste
		// ou s'il est consécutif à un autre opérateur logique.
		if (((curr->type == PIPE) && !prev) || (prev && (prev->type == PIPE)
				&& (curr->type == PIPE)))
			return (print_syntax_error(curr->str));
		// Sinon on avance dans la liste
		prev = curr;
		curr = curr->next;
	}
	// Si le dernier token est un opérateur logique,
	// on affiche une erreur de syntaxe.
	if (prev && (prev->type == PIPE))
		return (print_syntax_error(prev->str));
	return (0);
}
