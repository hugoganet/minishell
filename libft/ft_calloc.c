/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 10:05:09 by elaudrez          #+#    #+#             */
/*   Updated: 2024/12/03 11:05:46 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	unsigned char	*p;
	size_t			i;
	size_t			x;

	x = nmemb * size;
	if (nmemb == 0 || size == 0)
		return (malloc(0));
	p = malloc(nmemb * size);
	if (p == NULL)
		return (NULL);
	if (nmemb != (x / size))
		return (NULL);
	i = 0;
	while (i < nmemb * size)
	{
		p[i] = 0;
		i++;
	}
	return ((void *) p);
}
