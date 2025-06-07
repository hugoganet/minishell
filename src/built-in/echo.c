/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bernard <bernard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 20:14:00 by bernard           #+#    #+#             */
/*   Updated: 2025/06/06 13:03:34 by bernard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

Boucler sur ast, si node->type == CMD. 
regarder si built in 

Regarder quel built in. 

si echo

node -> right ou node left 
str -> si single quote, ecrire tout. 
str -> 

void    print_env(t_shell *data, t_ast *node)
{
    int i;

    i = 0;
    if (node->type == CMD && node->str == 'env')
    {
        while (data->env[i])
        {
            printf("%s\n", data->env[i]);
            i++;
        }
    }
}