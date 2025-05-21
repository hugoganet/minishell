/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printa.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 14:06:29 by elaudrez          #+#    #+#             */
/*   Updated: 2025/04/03 14:23:04 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_printa(unsigned long nb, char *base)
{	
	if (!nb)
	{
		write(1, "(nil)", 5);
		return (5);
	}
	write (1, "0x", 2);
	return (ft_putnbr_base1((unsigned long)nb, base) + 2);
}

/*int	main()
{
	char	*p;
	char	*base;
	// int	i;

	base = "0123456789abcdef";
	p = "k";
	ft_printa(*p, base);
	// printf("\n%d", i);
	// printf("%p", p);
}*/