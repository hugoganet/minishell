/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 17:31:39 by hugoganet         #+#    #+#             */
/*   Updated: 2025/05/23 17:31:44 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Affiche la liste des tokens avec leur type.
 *
 * @param head Début de la liste chaînée
 */
void print_tokens(t_token *head)
{
	while (head)
	{
		printf("[%-16s] → type = %d\n", head->value, head->type);
		head = head->next;
	}
}
