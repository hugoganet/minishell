/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 09:46:30 by elaudrez          #+#    #+#             */
/*   Updated: 2024/11/26 13:06:15 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	char	*s2;
	int		n;

	n = 0;
	while (s1[n])
	{
		n++;
	}
	s2 = malloc(n * sizeof(char) + 1);
	if (s2 == NULL)
		return (NULL);
	n = 0;
	while (s1[n])
	{
		s2[n] = s1[n];
		n++;
	}
	s2[n] = '\0';
	return (s2);
}
