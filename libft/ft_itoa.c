/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 18:18:04 by elaudrez          #+#    #+#             */
/*   Updated: 2024/12/03 13:37:36 by elaudrez         ###   ########.fr       */
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
	str = malloc((len + 1) * sizeof(char));
	if (!str)
		return (NULL);
	str[len] = '\0';
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
