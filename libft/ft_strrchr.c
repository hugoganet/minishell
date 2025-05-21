/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 18:05:11 by elaudrez          #+#    #+#             */
/*   Updated: 2024/11/29 18:59:39 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	while (i > 0)
	{
		if (i > 0 && s[i] == (unsigned char) c)
			return ((char *) &s[i]);
		i--;
	}
	if (s[i] == (unsigned char) c)
		return ((char *) &s[i]);
	return (NULL);
}

/*int	main()
{
	char	*s;

	s = "\0";
	printf("%s", ft_strrchr(s, 'l'));
}*/