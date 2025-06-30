/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   refine_token_type.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 13:13:56 by hugoganet         #+#    #+#             */
/*   Updated: 2025/06/30 13:05:23 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Affine les types des tokens `WORD` en `CMD` ou `ARG`.
 *
 * Cette fonction analyse la liste de tokens issue de la tokenisation initiale,
 * où tous les mots simples sont de type `WORD`. Elle les requalifie contextuellement
 * en `CMD` (commande) ou `ARG` (argument), selon leur position dans la ligne.
 *
 * @param head Pointeur vers le premier élément de la liste chaînée de tokens.
 */
void refine_token_types(t_token *head)
{
	t_token *curr;
	int expect_cmd;

	expect_cmd = 1;
	curr = head;
	while (curr)
	{
		// Ignore les redirections (leur type est déjà correct)
		if (is_redirection(curr->type))
		{
			curr = curr->next;
			continue;
		}
		// Classe les WORDs restants selon le contexte
		if (curr->type == WORD)
		{
			if (expect_cmd)
				curr->type = CMD;
			else
				curr->type = ARG;
		}
		// Mise à jour du contexte
		if (curr->type == PIPE)
			expect_cmd = 1;
		else if (curr->type == CMD || curr->type == ARG)
			expect_cmd = 0;
		curr = curr->next;
	}
}
