/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 17:47:12 by elaudrez          #+#    #+#             */
/*   Updated: 2024/12/03 13:38:41 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	unsigned char		*temp_dst;
	const unsigned char	*temp_src;

	if ((!src && !dst))
		return (NULL);
	temp_dst = (unsigned char *) dst;
	temp_src = (const unsigned char *) src;
	if (temp_dst > temp_src)
	{
		temp_dst += len -1;
		temp_src += len - 1;
		while (len--)
		{
			*temp_dst-- = *temp_src--;
		}
	}
	else
	{
		while (len--)
		{
			*temp_dst++ = *temp_src++;
		}
	}
	return (dst);
}
