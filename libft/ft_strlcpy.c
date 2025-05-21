/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 12:38:58 by elaudrez          #+#    #+#             */
/*   Updated: 2024/11/29 18:01:10 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	i;

	i = 0;
	if (dstsize != 0)
	{
		while (src[i] && i < dstsize - 1)
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = '\0';
	}
	else
	{
		if (!dst)
			dst[i] = '\0';
	}
	return (ft_strlen(src));
}

/*int	main()
{
	char	src[4] = "abcd";
	char	dst[3] = "efg";
	int	i = ft_strlcpy(dst, src, 8);
	printf("%d", i);
}*/