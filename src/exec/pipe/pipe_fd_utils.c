/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_fd_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 16:00:00 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/07 16:00:00 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipe.h"

/**
 * @brief Ferme tous les descripteurs de pipes dans le processus parent.
 *
 * Cette fonction est appelée dans le parent après le fork de tous les enfants du pipeline.
 * Elle ferme les deux descripteurs (lecture et écriture) de chaque pipe,
 * car le parent n'a plus besoin de garder ces pipes ouverts.
 *
 * @param pipes Tableau des pipes (tableaux de 2 descripteurs)
 * @param cmd_count Nombre total de commandes dans le pipeline
 */
void	close_parent_pipes(int **pipes, int cmd_count)
{
	int	i;

	i = 0;
	while (i < cmd_count - 1)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}
