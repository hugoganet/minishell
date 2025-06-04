/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_token_list.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 19:03:24 by hugoganet         #+#    #+#             */
/*   Updated: 2025/06/04 17:37:39 by hugoganet        ###   ########.fr       */
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

/**
 * @brief Libère un tableau de chaînes de caractères alloué dynamiquement.
 *
 * @param split Le tableau de chaînes de caractères à libérer.
 */
void free_split(char **split)
{
	int i;

	i = 0;
	while (split && split[i])
		free(split[i++]);
	free(split);
}