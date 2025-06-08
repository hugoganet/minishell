/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 16:59:32 by elaudrez          #+#    #+#             */
/*   Updated: 2025/06/08 18:51:24 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_cd(t_ast *node, t_shell *data)
{
	t_ast	*arg_node;
	char	*tmp;
	int		i;

	i = 0;
	arg_node = node->right;
	if (chdir(arg_node->str))
	{
		while (data->env[i])
		{
			if (ft_strncmp(data->env[i], "PWD=", 4) == 0)
				break ;
			i++;
		}
		ft_strcpy(tmp, data->env[i]); //Concerver pwd pour le mettre dans oldpwd apres
		getcwd(data->env[i], len de ) // copie chemin absolu du repertoire courant dans chaine pointe par buf, de longueur n_size
		i = 0;
		while (data->env[i])
		{
			if (ft_strncmp(data->env[i], "OLDPWD=", 7) == 0)
				break ;
			i++;
		}
		
	}
	
}
