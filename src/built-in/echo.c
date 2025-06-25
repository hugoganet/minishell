/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 20:14:00 by bernard           #+#    #+#             */
/*   Updated: 2025/06/24 17:59:48 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int   ft_echo(t_ast *node)
{
    int     print_nl;
    int     i;
    
    print_nl = 0;
    if (node && ft_strcmp(node->args[1], "-n") == 0)
    {
        if (!node->args[2])
            return (0);
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
   return (0);
}

//if_isquote ne pas print les quotes 