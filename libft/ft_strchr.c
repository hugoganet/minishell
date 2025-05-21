/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 16:31:17 by elaudrez          #+#    #+#             */
/*   Updated: 2024/12/03 12:14:09 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == (unsigned char) c)
			return ((char *) &s[i]);
		i++;
	}
	if ((unsigned char) c == '\0')
		return ((char *) &s[i]);
	if (s[i] == c)
		return (((char *) &s[i]));
	return (NULL);
}

/*int	main()
{
	const char	*s;

	s = "\0";
	printf("%s", ft_strchr(s, 'b'));
}*/