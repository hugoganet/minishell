/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 18:37:02 by elaudrez          #+#    #+#             */
/*   Updated: 2024/12/02 15:35:09 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*ss;
	unsigned char	cc;

	ss = (unsigned char *) s;
	cc = (unsigned char) c;
	i = 0;
	while (i < n)
	{
		if (ss[i] == cc)
			return ((void *) &ss[i]);
		i++;
	}
	return (NULL);
}

/*int	main(void)
{
	const void	*s;
	char	*ss;
	
	s = "koi";
	ss = ft_memchr(s, 105, 3);
	printf("%s", ss);
}*/