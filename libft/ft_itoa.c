/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 18:18:04 by elaudrez          #+#    #+#             */
/*   Updated: 2025/07/07 21:14:23 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	n_len(long nbr)
{
	int		i;

	i = 0;
	if (nbr < 0)
	{
		nbr = nbr * -1;
		i = 1;
	}
	if (nbr == 0)
		i = 1;
	while (nbr > 0)
	{
		nbr = nbr / 10;
		i++;
	}
	return (i);
}

char	*ft_itoa(int n)
{
	char	*str;
	long	len;
	long	nbr;

	nbr = (long)n;
	len = n_len(nbr);
	str = ft_calloc((len + 1), sizeof(char));
	if (!str)
		return (NULL);
	if (nbr < 0)
	{
		nbr = -nbr;
		str[0] = '-';
	}
	while (len > 0)
	{
		if (str[0] == '-' && nbr == 0)
			break ;
		str[len - 1] = nbr % 10 + '0';
		nbr = nbr / 10;
		len--;
	}
	return (str);
}
