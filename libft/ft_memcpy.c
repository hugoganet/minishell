/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 16:59:16 by elaudrez          #+#    #+#             */
/*   Updated: 2024/12/03 13:38:26 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char		*temp_dst;
	const unsigned char	*temp_src;
	size_t				i;

	if ((src == NULL && dst == NULL) || !n)
		return (dst);
	temp_dst = (unsigned char *) dst;
	temp_src = (unsigned char *) src;
	i = 0;
	while (i < n)
	{
		temp_dst[i] = temp_src[i];
		i++;
	}
	return (dst);
}
