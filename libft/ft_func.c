/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_func.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 15:37:01 by elaudrez          #+#    #+#             */
/*   Updated: 2025/04/03 14:24:53 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putchar(char c)
{
	write(1, &c, 1);
	return (1);
}

int	ft_putstr(char *str)
{
	int	i;

	i = 0;
	if (str == NULL)
		return (ft_putstr("(null)"));
	while (str[i])
	{
		ft_putchar(str[i]);
		i++;
	}
	return (i);
}

int	ft_putnbr_base1(unsigned long nb, char *base)
{
	size_t	i;
	size_t	base_l;

	base_l = ft_strlen(base);
	i = 0;
	if (base == 0)
		return (0);
	if (nb < 0)
	{
		ft_putchar('-');
		nb = -nb;
		i++;
	}
	if (nb >= (unsigned long long) base_l)
		i += ft_putnbr_base1(nb / base_l, base);
	i += ft_putchar(base[nb % base_l]);
	return (i);
}

int	ft_putnbr_base2(long nb, char *base)
{
	int	i;
	int	base_l;

	base_l = ft_strlen(base);
	i = 0;
	if (base == 0)
		return (0);
	if (nb < 0)
	{
		ft_putchar('-');
		nb = -nb;
		i++;
	}
	if (nb >= (long)base_l)
		i += ft_putnbr_base2(nb / base_l, base);
	i += ft_putchar(base[nb % base_l]);
	return (i);
}
