/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bernard <bernard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 12:42:15 by bernard           #+#    #+#             */
/*   Updated: 2025/06/18 12:04:01 by bernard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_is_valid(char *args)
{
    int i;

    i = 0;
    if (!args || (ft_is_alpha(key[0] && key[0] != '_')))
        return (0);
    while (args[i])
    {
        if (!ft_is_alnum(args[i]) && args[i] != '_')
            return (0);
        i++;
    }
    return (1);
}

void    remove_env_node(t_env **env, char *key)
{
    t_env   *current;
    t_env   *prev;

    current = *env;
    prv = NULL;
    while(current)
    {
        if(ft_strcmp(current->key, key) == 0)
        {
            if (prev == NULL)
                *env = current->next;
            else
                prev->next = current->next;
            free(current->value);
            free(current->key);
            free(current);
            return ;
        }
        prev = current;
        current = current->next;
    }
}

int ft_unset(t_env **env, t_ast *node)
{
    int i;

    i = 1;
    while(node->args[i])
    {
        if (ft_is_valid(node->args[i]))
            remove_env_node(*env, node->args[i]);
        else
            printf("unset : %s : not a valid identifier\n", node->args[i])
        i++;
    }
    return (0);
}