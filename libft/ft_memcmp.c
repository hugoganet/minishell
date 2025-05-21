/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 14:59:59 by elaudrez          #+#    #+#             */
/*   Updated: 2024/11/29 18:46:52 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t			i;
	unsigned char	*ss1;
	unsigned char	*ss2;

	i = 0;
	ss1 = (unsigned char *) s1;
	ss2 = (unsigned char *) s2;
	while (i < n)
	{
		if (!(ss1[i] == ss2[i]))
			return (ss1[i] - ss2[i]);
		i++;
	}
	return (0);
}

/*int	main()
{
	unsigned char	s1[] = "437p";
	unsigned char	s2[] = "43kd";

	printf("%d", ft_memcmp(s1, s2, 2));
}*/