/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 16:35:27 by elaudrez          #+#    #+#             */
/*   Updated: 2025/06/08 17:51:38 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int   ft_env(t_shell *data, t_ast *node)
{
    int i;

    i = 0;
    if (node->type == CMD && ft_strncmp(node->str,"env", 4) == 0)
    {
        while (data->env[i])
        {
            printf("%s\n", data->env[i]);
            i++;
        }
    }
    return (0);
}