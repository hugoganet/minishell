/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   refine_token_type.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 13:13:56 by hugoganet         #+#    #+#             */
/*   Updated: 2025/06/11 13:14:33 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Traite le cas d’une redirection en tout début de ligne.
 * @param curr Pointeur vers le pointeur courant dans la liste
 */
static void handle_initial_redirection(t_token **curr)
{
	if (is_redirection((*curr)->type) && (*curr)->next)
	{
		(*curr) = (*curr)->next;
		(*curr)->type = CMD;
	}
}

/**
 * @brief Classe le token suivant une redirection.
 * @param curr Pointeur vers le pointeur courant dans la liste
 * @param has_cmd 1 si une commande a déjà été rencontrée
 */
static void handle_redirection_context(t_token **curr, int has_cmd)
{
	if (is_redirection((*curr)->type) && (*curr)->next && (*curr)->next->type != PIPE)
	{
		(*curr) = (*curr)->next;
		if (has_cmd == 0)
			(*curr)->type = CMD;
		else
			(*curr)->type = ARG;
	}
}

/**
 * @brief Met à jour les drapeaux de contexte en fonction du type courant.
 * @param curr Token courant
 * @param expect_cmd Pointeur vers le drapeau d’attente de commande
 * @param has_cmd Pointeur vers le drapeau de commande déjà rencontrée
 */
static void update_context_flags(t_token *curr, int *expect_cmd, int *has_cmd)
{
	if (curr->type == PIPE)
	{
		*expect_cmd = 1;
		*has_cmd = 0;
	}
	if (curr->type == CMD || curr->type == ARG)
		*expect_cmd = 0;
	if (curr->type == CMD)
		*has_cmd = 1;
}

/**
 * @brief Reclasse les tokens WORD en CMD, ARG ou FILES selon leur contexte.
 * @param head Pointeur vers la tête de la liste chaînée de tokens
 */
void refine_token_types(t_token *head)
{
	t_token *curr;
	int expect_cmd;
	int has_cmd;

	curr = head;
	expect_cmd = 1;
	has_cmd = 0;
	handle_initial_redirection(&curr);
	while (curr)
	{
		handle_redirection_context(&curr, has_cmd);
		if (curr->type == WORD)
			curr->type = (expect_cmd ? CMD : ARG);
		update_context_flags(curr, &expect_cmd, &has_cmd);
		curr = curr->next;
	}
}