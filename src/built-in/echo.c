/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bernard <bernard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 20:14:00 by bernard           #+#    #+#             */
/*   Updated: 2025/06/16 12:52:09 by bernard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int   ft_echo(t_ast *node)
{
    int     print_nl;
    int     i;
    
    print_nl = 0;
    if (node && ft_strncmp(node->args[1], "-n", 2))
    {
        if (!node->args[2])
            return (NULL);
        i = 2;
        print_nl = 1;
    }
    else
    {
        i = 1;
        while (node->args[i])
        {
            printf("%s", node->args[i]);
            printf(" ");
            i++;
        }
    }
    if (!print_nl)
        printf("\n");
   return 0;
}

