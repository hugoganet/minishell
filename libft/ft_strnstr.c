/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 09:39:46 by elaudrez          #+#    #+#             */
/*   Updated: 2024/12/02 16:05:13 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	if (needle[0] == 0)
		return ((char *) haystack);
	while (haystack[i] && i < len)
	{
		j = 0;
		while (haystack[i + j] == needle[j] && haystack[i + j] && i + j < len)
		{
			j++;
			if (needle[j] == '\0')
				return ((char *) &haystack[i]);
		}
		i++;
		j = 0;
	}
	return (0);
}

/*int	main()
{
	const char	*haystack;
	const char	*needle;
	
	haystack = "Bonjour Madame ca va bien";
	needle = "Madame";
	printf("%s", ft_strnstr(haystack, needle, 14));
}*/