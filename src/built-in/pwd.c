/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 17:41:35 by bernard           #+#    #+#             */
/*   Updated: 2025/06/23 17:33:33 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_pwd(t_ast *node, t_shell *data)
{
    char    *buff[PATH_MAX];

    if (getcwd(buff, PATH_MAX))
    {
        printf("%s\n", buff);
        return (0);
    }
    else
        perror("getcwd");
    return (1);
}
