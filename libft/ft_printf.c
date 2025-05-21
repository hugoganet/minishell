/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 11:54:43 by elaudrez          #+#    #+#             */
/*   Updated: 2025/04/03 14:22:58 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_type(va_list args, const char *format)
{
	int	re;

	re = 0;
	if (*format == 'c')
		re = (ft_putchar(va_arg(args, int)));
	if (*format == 's')
		re = (ft_putstr(va_arg(args, char *)));
	if (*format == 'p')
		re = (ft_printa(va_arg(args, unsigned long long), "0123456789abcdef"));
	if (*format == 'd' || *format == 'i')
		re = (ft_putnbr_base2(va_arg(args, int), "0123456789"));
	if (*format == 'u')
		re = (ft_putnbr_base1(va_arg(args, unsigned int), "0123456789"));
	if (*format == 'x')
		re = (ft_putnbr_base1(va_arg(args, unsigned int), "0123456789abcdef"));
	if (*format == 'X')
		re = (ft_putnbr_base1(va_arg(args, unsigned int), "0123456789ABCDEF"));
	if (*format == '%')
	{
		write(1, "%", 1);
		re = 1;
	}
	return (re);
}

int	ft_printf(const char *format, ...)
{
	int		count;
	va_list	args;

	count = 0;
	if (!format)
		return (-1);
	va_start(args, format);
	while (*format)
	{
		if (*format == '%')
		{
			format++;
			count += ft_type(args, format);
			format++;
		}
		else
		{
			ft_putchar(*format);
			count++;
			format++;
		}
	}
	va_end(args);
	return (count);
}
