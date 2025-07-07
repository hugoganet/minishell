/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 15:52:26 by elaudrez          #+#    #+#             */
/*   Updated: 2025/07/07 21:13:20 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *b, int c, size_t len)
{
	unsigned char	*temp;
	size_t			i;

	temp = (unsigned char *) b;
	i = 0;
	while (i < len)
	{
		temp[i] = (unsigned char) c;
		i++;
	}
	return (b);
}
/*int	main()
{
	unsigned char	b[] = "kjdll";
	ft_memset(b, 95, 3);
	printf("%s", b);
	return(0);
}*/