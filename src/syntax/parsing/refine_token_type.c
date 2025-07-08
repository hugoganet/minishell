/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   refine_token_type.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 13:13:56 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/09 11:10:07 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "syntax.h"

/**
 * @brief Affine les types des tokens `WORD` en `CMD` ou `ARG`.
 *
 * Cette fonction analyse la liste de tokens issue de la tokenisation initiale,
 * où tous les mots simples sont de type `WORD`. Elle les requalifie
 * contextuellement
 * en `CMD` (commande) ou `ARG` (argument), selon leur position dans la ligne.
 *
 * Règles appliquées :
 * 
 * - Le premier WORD devient CMD
 * 
 * - Les WORD après PIPE deviennent CMD
 * 
 * - Les autres WORD deviennent ARG
 * 
 * - Les redirections sont ignorées dans le processus
 *
 * @param head Pointeur vers le premier élément de la liste chaînée de tokens.
 */
void	refine_token_types(t_token *head)
{
	t_token	*curr;
	int		expect_cmd;

	// On initialise `expect_cmd` à 1 pour indiquer que le prochain
	// token de type WORD doit être traité comme une commande (CMD).
	expect_cmd = 1;
	curr = head;
	// On parcourt la liste de tokens
	while (curr)
	{
		// Si on rencontre une redirection, on l'ignore
		if (is_redirection(curr->type))
		{
			curr = curr->next;
			continue ;
		}
		// Si le token est de type WORD, on le requalifie
		if (curr->type == WORD)
		{
			// Si on attend une commande, on le marque comme CMD
			if (expect_cmd)
				curr->type = CMD;
			// Sinon, on le marque comme ARG
			else
				curr->type = ARG;
		}
		// Si le token est de type PIPE, on s'attend à ce que le prochain
		// token de type WORD soit une commande (CMD).
		if (curr->type == PIPE)
			expect_cmd = 1;
		// Si le token est de type CMD ou ARG, on ne s'attend plus à une
		// commande, donc on met `expect_cmd` à 0.
		else if (curr->type == CMD || curr->type == ARG)
			expect_cmd = 0;
		curr = curr->next;
	}
}
