/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 08:25:59 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/04 10:56:09 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"

/**
 * @brief Initialise les signaux dans le processus enfant.
 *
 * Permet à l'enfant de recevoir normalement les signaux SIGINT et SIGQUIT.
 */
void reset_signals_in_child(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

/**
 * @brief Affiche le message d'erreur approprié pour une commande non trouvée.
 *
 * @param cmd_name Nom de la commande
 */
void print_command_not_found_error(char *cmd_name)
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
}

/**
 * @brief Vérifie si le chemin pointe vers un répertoire.
 *
 * @param path Chemin à vérifier
 * @return 1 si c'est un répertoire, 0 sinon
 */
int is_directory(char *path)
{
	struct stat path_stat;

	if (stat(path, &path_stat) == 0)
	{
		if (S_ISDIR(path_stat.st_mode))
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(path, STDERR_FILENO);
			ft_putendl_fd(": is a directory", STDERR_FILENO);
			return (1);
		}
	}
	return (0);
}
