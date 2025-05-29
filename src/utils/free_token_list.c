/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_token_list.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 19:03:24 by hugoganet         #+#    #+#             */
/*   Updated: 2025/05/29 17:54:54 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Libère toute la liste de tokens allouée par le parsing.
 *
 * @param head Le pointeur vers le premier token de la liste.
 */
void free_token_list(t_token *head)
{
	t_token *tmp;

	// Tant qu'il y a des tokens dans la liste
	while (head)
	{
		// Sauvegarde le pointeur vers le prochain token
		tmp = head->next;
		// Libère la valeur du token
		free(head->str);
		// Libère le token lui-même
		free(head);
		// Avance au prochain token
		head = tmp;
	}
}