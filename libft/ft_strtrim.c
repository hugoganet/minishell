/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 16:59:14 by elaudrez          #+#    #+#             */
/*   Updated: 2024/12/04 17:10:01 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	check_char(char const *set, char c)
{
	size_t	i;

	i = 0;
	while (set[i])
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*str;
	size_t	i;
	size_t	start;
	size_t	end;

	start = 0;
	i = 0;
	if (s1[0] == '\0')
		return (ft_calloc(sizeof(char), 1));
	end = ft_strlen(s1) - 1;
	while (check_char(set, s1[start]) == 1 && start <= end)
		start++;
	while (check_char(set, s1[end]) == 1 && end > start)
		end--;
	str = ft_calloc(sizeof(char), (end - start + 2));
	if (!str)
		return (NULL);
	while (start <= end)
	{
		str[i] = s1[start];
		i++;
		start++;
	}
	str[i] = 0;
	return (str);
}
