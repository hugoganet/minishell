/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logic_groups.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 19:48:36 by elaudrez          #+#    #+#             */
/*   Updated: 2025/07/04 09:41:59 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "syntax.h"

/**
 * @brief Marque le premier token WORD comme CMD si présent.
 *
 * @param ptr Pointeur vers le token courant
 * @return Pointeur vers le token suivant à traiter
 */
static t_token *set_first_command(t_token *ptr)
{
	if (ptr != NULL && ptr->type == WORD)
	{
		ptr->type = CMD;
		ptr = ptr->next;
	}
	return (ptr);
}

/**
 * @brief Traite les opérateurs logiques et redirections.
 *
 * Marque les tokens suivant PIPE comme CMD et les tokens suivant
 * les redirections comme FILES.
 *
 * @param ptr Pointeur vers le premier token à traiter
 */
static void process_operators_and_redirections(t_token *ptr)
{
	while (ptr != NULL)
	{
		if (ptr->type == PIPE && ptr->next)
		{
			ptr = ptr->next;
			ptr->type = CMD;
		}
		else if ((ptr->type == REDIR_INPUT || ptr->type == REDIR_APPEND ||
				  ptr->type == REDIR_OUTPUT || ptr->type == HEREDOC) &&
				 ptr->next)
		{
			ptr = ptr->next;
			ptr->type = FILES;
		}
		ptr = ptr->next;
	}
}

/**
 * @brief Marque les arguments suivant les commandes.
 *
 * Parcourt la liste et transforme les tokens WORD suivant
 * immédiatement une commande en arguments (ARG).
 *
 * @param node Pointeur vers le début de la liste de tokens
 */
static void set_command_arguments(t_token *node)
{
	t_token *ptr;

	ptr = node;
	while (ptr != NULL)
	{
		if (ptr->type == CMD && ptr->next && ptr->next->type == WORD)
		{
			ptr = ptr->next;
			ptr->type = ARG;
		}
		ptr = ptr->next;
	}
}

/**
 * @brief Analyse et classe les tokens selon leur rôle logique.
 *
 * Cette fonction parcourt une liste de tokens et les reclassifie selon
 * leur position et contexte :
 * - Le premier WORD devient CMD
 * - Les WORD après PIPE deviennent CMD
 * - Les WORD après redirections deviennent FILES
 * - Les WORD après CMD deviennent ARG
 *
 * @param node Pointeur vers le pointeur de début de liste de tokens
 * @return Pointeur vers le début de la liste modifiée
 */
t_token *is_logic(t_token **node)
{
	t_token *ptr;

	ptr = *node;
	ptr = set_first_command(ptr);
	process_operators_and_redirections(ptr);
	set_command_arguments(*node);
	return (*node);
}
