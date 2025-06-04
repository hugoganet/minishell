/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bernard <bernard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 17:32:00 by elaudrez          #+#    #+#             */
/*   Updated: 2025/06/04 19:09:04 by bernard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_is_sgl_quote(char c)
{
	int	i;

	i = 0;
	if (c == 39)
		i = 1;
	return (i);
}

int	ft_is_dbl_quote(char c)
{
	int	i;

	i = 0;
	if (c == 34)
		i = 1;
	return (i);
}

int	which_quote(t_ast *node)
{
	if (node->str[0] == 39)
		return (0);
	return (1);
}

char	*ft_strcpy(char *dest, char *src)
{
	int	i;
	
	i = 0;
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}