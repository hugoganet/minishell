// ! Penser à appeler cleanup_shell(shell) avant de quitter le shell
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 14:24:18 by bernard           #+#    #+#             */
/*   Updated: 2025/06/24 11:26:57 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_isnum(t_ast *node)
{
	int i;

	i = 0;
	if (node->args[1][i] == '-' || node->args[1][i] == '+')
        i++;
    while(ft_isdigit(node->args[1][i]))
    {
        i
    }
	return()
	
}

int    ft_exit(t_shell *data, t_ast *node)
{
   
}

int ft_exit(t_shell *data, t_ast *node)
{
    unsigned int    status;

	status = EXIT_FAILURE;
	
	
}

