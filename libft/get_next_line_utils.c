/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 12:01:56 by elaudrez          #+#    #+#             */
/*   Updated: 2025/03/20 13:36:46 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*free_strjoin(char *s1, char *s2)
{
	char	*temp;

	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
	{
		free (s1);
		return (NULL);
	}	
	temp = ft_strjoin(s1, s2);
	free(s1);
	return (temp);
}
