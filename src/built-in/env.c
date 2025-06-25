/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 16:35:27 by elaudrez          #+#    #+#             */
/*   Updated: 2025/06/25 11:10:46 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int   ft_env(t_ast *node, t_shell *data)
{
    // int i;
    t_env   *curr;

    // i = 0;
    curr = data->env_list;
    if (node->type == CMD && ft_strcmp(node->str,"env") == 0)
    {
        while (curr)
        {
            printf("%s", curr->key);
            printf("=");
            printf("%s\n", curr->value);
            curr = curr->next;
        }
    }
    return (0);
}