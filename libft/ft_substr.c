/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 18:32:06 by elaudrez          #+#    #+#             */
/*   Updated: 2024/12/03 11:46:57 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char			*s2;
	unsigned int	i;
	size_t			s_len;
	size_t			sturt;

	s_len = ft_strlen(s);
	sturt = (size_t) start;
	if (!s)
		return (NULL);
	if (sturt > s_len)
		return (ft_strdup(""));
	if (len > s_len - sturt)
		len = s_len - sturt;
	s2 = ft_calloc((len + 1), 1);
	if (! s2)
		return (NULL);
	i = 0;
	while (i < len)
	{
		s2[i] = s[sturt + i];
		i++;
	}
	s2[i] = '\0';
	return (s2);
}

/*int	main()
{
	char const	*s = "kio ca va";
	
	printf("%s", ft_substr(s, 1, 4));
	return (0);
}*/