/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 15:52:26 by elaudrez          #+#    #+#             */
/*   Updated: 2024/11/29 18:51:41 by elaudrez         ###   ########.fr       */
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