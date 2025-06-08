/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 20:14:00 by bernard           #+#    #+#             */
/*   Updated: 2025/06/08 16:51:37 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int   ft_echo(t_ast *node)
{
    t_ast   *arg_node;
    int     print_nl;
    
    print_nl = 0;
    arg_node = node->right;
    if (arg_node && ft_strncmp(arg_node->str, "-n", 2)) // Si il n'y a pas de node->right, alors arg_node == NULL et on ne rentre pas dans les boucles et on print direct un \n
    {
        if (!node->right->right)
            return (NULL);
        node = node->right->right;
        printf("%s", node->str);
        print_nl = 1;
    }
    while (arg_node)
    {
        printf("%s", arg_node->str);
        if (arg_node->right)
        {
            printf(" ");
            arg_node = arg_node->right;
        }
    }
    if (print_nl)
        printf("\n");
   return 0;
}

