/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 15:44:24 by elaudrez          #+#    #+#             */
/*   Updated: 2024/12/03 13:28:13 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	j;
	size_t	lsrc;

	i = ft_strlen(dst);
	lsrc = ft_strlen(src);
	j = 0;
	if (dstsize <= i)
		return (lsrc + dstsize);
	while (src[j] && (i + j) < dstsize - 1)
	{
			dst[i + j] = src[j];
			j++;
	}
	dst[i + j] = '\0';
	return (i + lsrc);
}
