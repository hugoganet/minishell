/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_complex_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 16:00:00 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/07 16:00:00 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipe.h"


/**
 * @brief Crée un processus enfant pour une commande du pipeline.
 *
 * Fork un nouveau processus et exécute la commande correspondante.
 * En cas d'échec du fork, termine les processus déjà créés.
 *
 * @param ctx Contexte du pipeline (commandes, pipes, pids, etc.)
 * @return 0 si succès, 1 en cas d'erreur de fork
 */
int create_child_process(t_pipeline_context *ctx)
{
	// On fork un nouveau processus pour exécuter la commande du pipeline.
	ctx->pids[ctx->current_index] = fork();
	// Si le fork échoue, on affiche une erreur et on termine les processus déjà créés.
	if (ctx->pids[ctx->current_index] < 0)
	{
		perror("minishell: fork");
		terminate_child_processes(ctx->pids, ctx->current_index);
		return (1);
	}
	if (ctx->pids[ctx->current_index] == 0)
	{
		// On est dans le processus enfant, on exécute directement la commande du pipeline.
		execute_pipeline_child(ctx);
	}
	return (0);
}

/**
 * @brief Crée tous les processus enfants pour exécuter le pipeline.
 *
 * Parcourt toutes les commandes du pipeline, fork chaque processus et gère les erreurs.
 *
 * @param ctx Contexte du pipeline (commandes, pipes, pids, etc.)
 * @return 0 si succès, 1 en cas d'erreur de fork
 */
int create_pipeline_processes(t_pipeline_context *ctx)
{
	int i;

	// Initialisation des PIDs à -1 pour chaque commande du pipeline
	initialize_pipeline_pids(ctx->pids, ctx->cmd_count);
	i = 0;
	// On parcourt le tableau des commandes et on crée un processus pour chaque commande
	while (i < ctx->cmd_count)
	{
		ctx->current_index = i;
		// On passe à create_child_process le contexte complet
		// Le current_index indique quelle commande exécuter.
		if (create_child_process(ctx) != 0)
			return (1);
		i++;
	}
	return (0);
}

/**
 * @brief Attend la fin de tous les processus du pipeline et retourne le statut final.
 *
 * Attend chaque processus enfant, et retourne le code de sortie de la dernière commande.
 *
 * @param pids Tableau des PIDs des processus enfants
 * @param cmd_count Nombre de commandes/processus dans le pipeline
 * @return Code de retour de la dernière commande du pipeline
 */
int wait_for_all_processes(pid_t *pids, int cmd_count)
{
	int i;
	int status;
	int final_status;

	i = 0;
	final_status = 0;
	while (i < cmd_count)
	{
		// On attend chaque processus enfant et on récupère son statut de sortie.
		waitpid(pids[i], &status, 0);
		if (i == cmd_count - 1)
			// On ne traite le statut que pour le dernier processus.
			final_status = handle_process_exit_status(status);
		i++;
	}
	return (final_status);
}
