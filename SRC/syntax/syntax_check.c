/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 14:01:02 by hugoganet         #+#    #+#             */
/*   Updated: 2025/05/23 14:01:13 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Vérifie si une ligne est vide (ne contient que des espaces/tabs).
 *
 * @param input La ligne entrée par l’utilisateur.
 * @return int 1 si vide, 0 sinon.
 */
int is_line_empty(char *input)
{
	int i;

	if (!input)
		return (1);
	i = 0;
	while (input[i])
	{
		if (input[i] != ' ' && input[i] != '\t')
			return (0);
		i++;
	}
	return (1);
}