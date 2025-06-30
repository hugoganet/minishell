/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 16:35:27 by elaudrez          #+#    #+#             */
/*   Updated: 2025/06/30 15:18:13 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int   ft_env(t_ast *node, t_shell *data)
{
    t_env   *curr;

    if (!data->env_list)
        return (1);
    curr = data->env_list;
    if (node->args[1])
    {
        ft_putstr_fd("Subject : env with no options or arguments\n", STDERR_FILENO);
        return (127);
    }
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