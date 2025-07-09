/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_cleanup.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 16:00:00 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/07 16:00:00 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipe.h"

/**
 * @brief Libère toutes les ressources d'un pipeline (pipes, commandes, pids).
 *
 * Ferme et libère tous les pipes créés, puis libère les tableaux de commandes
 * et de pids.
 *
 * @param ctx Contexte du pipeline contenant toutes les ressources
 */
void	cleanup_pipeline_resources(t_pipeline_context *ctx)
{
	int	i;

	i = 0;
	if (ctx->pipes)
	{
		while (i < ctx->pipes_created)
		{
			if (ctx->pipes[i])
			{
				close(ctx->pipes[i][0]);
				close(ctx->pipes[i][1]);
				free(ctx->pipes[i]);
			}
			i++;
		}
		free(ctx->pipes);
	}
	if (ctx->commands)
		free(ctx->commands);
	if (ctx->pids)
		free(ctx->pids);
}

/**
 * @brief Libère uniquement la mémoire des tableaux du pipeline (sans
 * fermer les pipes).
 *
 * Libère les tableaux de pipes, commandes et pids, mais ne ferme pas
 * les descripteurs de pipe.
 *
 * @param ctx Contexte du pipeline contenant toutes les ressources
 */
void	cleanup_pipeline_memory_only(t_pipeline_context *ctx)
{
	int	i;

	i = 0;
	if (ctx->pipes)
	{
		while (i < ctx->pipes_created)
		{
			if (ctx->pipes[i])
				free(ctx->pipes[i]);
			i++;
		}
		free(ctx->pipes);
	}
	if (ctx->commands)
		free(ctx->commands);
	if (ctx->pids)
		free(ctx->pids);
}

/**
 * @brief Libère la mémoire des enfants.
 *
 * Utilisé pour nettoyer la mémoire allouée si un fork échoue avant
 * l'exécution du pipeline.
 *
 * @param commands Tableau des nœuds de commande du pipeline
 * @param pipes Tableau des pipes (tableaux de 2 descripteurs)
 * @param pids Tableau des PIDs des processus enfants
 * @param pipes_created Nombre de pipes effectivement créés
 */
void	cleanup_child_memory_early(t_ast **commands, int **pipes, pid_t *pids,
	int pipes_created)
{
	int	i;

	i = 0;
	if (pipes)
	{
		while (i < pipes_created)
		{
			if (pipes[i])
				free(pipes[i]);
			i++;
		}
		free(pipes);
	}
	if (commands)
		free(commands);
	if (pids)
		free(pids);
}

/**
 * @brief Termine tous les processus enfants du pipeline.
 *
 * Envoie SIGTERM à chaque PID valide, puis attend leur terminaison avec waitpid.
 *
 * @param pids Tableau des PIDs des processus enfants
 * @param count Nombre de processus à terminer
 */
void	terminate_child_processes(pid_t *pids, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if (pids[i] > 0)
			kill(pids[i], SIGTERM);
		i++;
	}
	i = 0;
	while (i < count)
	{
		if (pids[i] > 0)
			waitpid(pids[i], NULL, 0);
		i++;
	}
}

/**
 * @brief Initialise le tableau des PIDs du pipeline à -1.
 *
 * Met chaque entrée du tableau pids à -1 pour indiquer qu'aucun processus
 * n'est encore forké.
 *
 * @param pids Tableau des PIDs à initialiser
 * @param cmd_count Nombre de commandes/processus dans le pipeline
 */
void	initialize_pipeline_pids(pid_t *pids, int cmd_count)
{
	int	i;

	i = 0;
	while (i < cmd_count)
	{
		pids[i] = -1;
		i++;
	}
}
