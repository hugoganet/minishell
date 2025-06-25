/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 18:18:04 by elaudrez          #+#    #+#             */
/*   Updated: 2025/06/25 11:49:03 by hugoganet        ###   ########.fr       */
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
	// str = malloc((len + 1) * sizeof(char));
	str = ft_calloc((len + 1), sizeof(char));
	// Conditional jump or move depends on uninitialised value(s)
	//    at 0x10EE68: ft_itoa (in /home/hganet/minishell/Minishell)
	//    by 0x10C4D3: increment_shlvl (var_utils.c:84)
	//    by 0x1092BB: init_shell (initialisation.c:43)
	//    by 0x10919B: main (main.c:35)
	//  Uninitialised value was created by a heap allocation
	//    at 0x4885250: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-arm64-linux.so)
	//    by 0x10EE0B: ft_itoa (in /home/hganet/minishell/Minishell)
	//    by 0x10C4D3: increment_shlvl (var_utils.c:84)
	//    by 0x1092BB: init_shell (initialisation.c:43)
	//    by 0x10919B: main (main.c:35)
	if (!str)
	return (NULL);
	// str[len] = '\0';
	if (nbr < 0)
	{
		nbr = -nbr;
		str[0] = '-';
	}
	while (len > 0)
	{
		// ! Si nbr > 0, on lit str[0] avant qu'il ne soit initialisé
		if (str[0] == '-' && nbr == 0)
			break ;
		str[len - 1] = nbr % 10 + '0';
		nbr = nbr / 10;
		len--;
	}
	return (str);
}
