/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_exec_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 16:33:31 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/07 22:04:50 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"

/**
 * @brief Affiche le message d'erreur approprié pour une commande non trouvée.
 *
 * @param cmd_name Nom de la commande
 */
void	print_command_not_found_error(char *cmd_name)
{
	if (ft_strchr(cmd_name, '/'))
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		perror(cmd_name);
	}
	else
	{
		ft_putstr_fd(cmd_name, STDERR_FILENO);
		ft_putendl_fd(": command not found", STDERR_FILENO);
	}
	fflush(stderr);
}
