/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 13:39:35 by elaudrez          #+#    #+#             */
/*   Updated: 2025/07/07 21:13:08 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_isdelim(char s, char c)
{
	if (s == c)
		return (1);
	return (0);
}

static int	ft_countword(const char *s, char c)
{
	int	i;
	int	nbm;

	i = 0;
	nbm = 0;
	while (s[i])
	{
		while (s[i] && ft_isdelim(s[i], c))
			i++;
		if (s[i] && !ft_isdelim(s[i], c))
		{
			nbm++;
			i++;
			while (s[i] && !ft_isdelim(s[i], c))
				i++;
		}
	}
	return (nbm);
}

static char	*newstring(const char *s, int *start, char c)
{
	char	*str;
	int		i;
	int		end;

	end = 0;
	while (s[*start + end] && !ft_isdelim(s[*start + end], c))
		end++;
	str = malloc((end + 1) * sizeof(char));
	if (!str)
		return (NULL);
	i = 0;
	while (i < end)
	{
		str[i] = s[*start + i];
		i++;
	}
	str[i] = '\0';
	*start += end;
	return (str);
}

static void	ft_freestr(char **s, int index)
{
	int	i;

	i = 0;
	while (i < index)
	{
		free(s[i]);
		i++;
	}
	free(s);
}

char	**ft_split(const char *s, char c)
{
	char	**sp;
	int		i;
	int		index;

	i = 0;
	index = 0;
	sp = malloc((ft_countword(s, c) + 1) * sizeof(char *));
	if (!sp)
		return (NULL);
	while (s[i])
	{
		while (s[i] && ft_isdelim(s[i], c))
			i++;
		if (s[i] && !ft_isdelim(s[i], c))
		{
			sp[index++] = newstring(s, &i, c);
			if (!sp[index - 1])
			{
				ft_freestr(sp, index - 1);
				return (NULL);
			}
		}
	}
	sp[index] = NULL;
	return (sp);
}
