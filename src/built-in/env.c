/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 16:35:27 by elaudrez          #+#    #+#             */
/*   Updated: 2025/06/26 15:32:07 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int   ft_env(t_ast *node, t_shell *data)
{
    // int i;
    t_env   *curr;

    // i = 0;
    if (!data->env_list)
        return (1);
    curr = data->env_list;
    if (node->type == CMD && ft_strcmp(node->args[0],"env") == 0)
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