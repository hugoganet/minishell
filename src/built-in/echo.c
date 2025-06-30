/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 20:14:00 by bernard           #+#    #+#             */
/*   Updated: 2025/06/27 16:56:45 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int valid_option(char *str)
{
    int i;

    i = 0;
    if (str[i] && str[i + 1] && str[i] == '-')
    {
        i++;
        while(str[i] && str[i] == 'n')
            i++;
        if (i == (int)ft_strlen(str))
            return (1);
    }
    return (0);
}

void print_echo(char *str)
{
    int i;

    i = 0;
    while(str[i])
    {
        if (str[i] == '\\')
            i++;
        printf("%c", str[i]);
        i++;
    }
}


int   ft_echo(t_ast *node)
{
    int     print_nl;
    int     i;
    
    print_nl = 0;
    i = 1;
    if (node && node->args[1] && valid_option(node->args[i]))
    {
        i = 2;
        print_nl = 1;
        while(node->args[i] && valid_option(node->args[i]))
            i++;
    }
    while (node->args[i])
    {
        print_echo(node->args[i]);
       if (node->args[i] && node->args[i + 1])
            printf(" ");
        i++;
    }
    if (!print_nl)
        printf("\n");
   return (0);
}
