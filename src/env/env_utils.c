/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 18:06:50 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/07 13:57:21 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Compare deux chaînes de caractères.
 * 
 * @param s1 Première chaîne à comparer
 * @param s2 Deuxième chaîne à comparer
 * @return Un entier négatif si s1 < s2, 0 si s1 == s2, un entier
 * positif si s1 > s2.
 */
int	ft_strcmp(char *s1, const char *s2)
{
	while (*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return (*s1 - *s2);
}
