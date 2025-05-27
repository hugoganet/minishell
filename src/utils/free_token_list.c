/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_token_list.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 19:03:24 by hugoganet         #+#    #+#             */
/*   Updated: 2025/05/23 19:04:17 by hugoganet        ###   ########.fr       */
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

	while (head)
	{
		tmp = head->next; // Sauvegarde le pointeur vers le prochain token
		free(head->value); // Libère la valeur du token
		free(head); // Libère le token lui-même
		head = tmp; // Avance au prochain token
	}
}