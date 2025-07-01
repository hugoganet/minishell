/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_token_list.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 18:19:01 by hugoganet         #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/05/28 12:31:03 by hugoganet        ###   ########.fr       */
=======
/*   Updated: 2025/06/04 13:49:03 by hugoganet        ###   ########.fr       */
>>>>>>> 0d1abe97ac7ef2f7c758effeb70145845d38e387
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Retourne le code couleur ANSI associé à un type de token.
 *
 * @param type Le type de token (CMD, ARG, REDIR_OUTPUT, etc.)
 * @return const char* Code couleur ANSI
 */
const char *token_color(t_token_type type)
{
	// Pipe = vert
	if (type == PIPE)
		return (COLOR_PIPE);
	// CMD = cyan
	if (type == CMD)
		return (COLOR_CMD);
	// ARG = bleu
	if (type == ARG)
		return (COLOR_ARG);
	if (type == HEREDOC)
		return (COLOR_HEREDOC);
	// Redirections = magenta
	if (type == REDIR_INPUT || type == REDIR_OUTPUT || type == REDIR_APPEND)
		return (COLOR_REDIR);
	if (type == FILES)
		return (COLOR_FILES);
	// Autres types = pas de couleur
	return (COLOR_RESET);
}

const char *token_type_str(t_token_type type)
{
	if (type == WORD)
		return ("WORD");
	if (type == CMD)
		return ("CMD");
	if (type == ARG)
		return ("ARG");
	if (type == FILES)
		return ("FILES");
	if (type == PIPE)
		return ("PIPE");
	if (type == REDIR_INPUT)
		return ("REDIR_INPUT");
	if (type == REDIR_OUTPUT)
		return ("REDIR_OUTPUT");
	if (type == REDIR_APPEND)
		return ("REDIR_APPEND");
	if (type == HEREDOC)
		return ("HEREDOC");
	return ("UNKNOWN");
}

/**
 * @brief Affiche chaque token de la liste avec son type
 *
 * @param tokens Liste chaînée de tokens
 * @param title Titre pour la section d'affichage
 */
void print_token_list(t_token *tokens, char *title)
{
	printf("\n=== %s ===\n", title);
	while (tokens)
	{
		printf(" - %-10s → %s%s%s\n", 
			tokens->str, 
			token_color(tokens->type) , token_type_str(tokens->type), COLOR_RESET);
		tokens = tokens->next;
	}
	printf("=========================\n\n");
}
