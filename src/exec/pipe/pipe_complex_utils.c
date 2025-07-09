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
 * @brief Prépare le contexte d'exécution pour un processus enfant du pipeline.
 *
 * Remplit la structure t_pipeline_child_ctx avec les informations nécessaires à l'exécution
 * d'une commande du pipeline (commandes, pipes, pids, index, env, shell).
 *
 * @param ctx Pointeur vers la structure de contexte à remplir
 * @param params Paramètres du pipeline (commandes, pipes, pids, etc.)
 */
void setup_child_context(t_pipeline_child_ctx *ctx, t_pipeline_params *params)
{
	ctx->commands = params->commands;
	ctx->pipes = params->pipes;
	ctx->pids = params->pids;
	ctx->cmd_count = params->cmd_count;
	ctx->index = params->index;
	ctx->env = params->env;
	ctx->shell = params->shell;
}

/**
 * @brief Crée un processus enfant pour une commande du pipeline.
 *
 * Fork un nouveau processus, prépare le contexte et exécute la commande correspondante.
 * En cas d'échec du fork, termine les processus déjà créés.
 *
 * @param params Paramètres du pipeline (commandes, pipes, pids, etc.)
 * @return 0 si succès, 1 en cas d'erreur de fork
 */
int create_child_process(t_pipeline_params *params)
{
	t_pipeline_child_ctx ctx;

	// On fork un nouveau processus pour exécuter la commande du pipeline.
	params->pids[params->index] = fork();
	// Si le fork échoue, on affiche une erreur et on termine les processus déjà créés.
	if (params->pids[params->index] < 0)
	{
		perror("minishell: fork");
		terminate_child_processes(params->pids, params->index);
		return (1);
	}
	if (params->pids[params->index] == 0)
	{
		// On est dans le processus enfant, on prépare le contexte d'exécution.
		// On remplit la structure t_pipeline_child_ctx avec les informations nécessaires à l'exécution
		// d'une commande du pipeline (commandes, pipes, pids, index, env, shell).
		// ! En gros, on copie tous les paramètres du pipeline dans le contexte enfant.
		setup_child_context(&ctx, params);
		// On exécute la commande du pipeline dans le processus enfant.
		execute_pipeline_child(&ctx);
	}
	return (0);
}

/**
 * @brief Crée tous les processus enfants pour exécuter le pipeline.
 *
 * Parcourt toutes les commandes du pipeline, fork chaque processus et gère les erreurs.
 *
 * @param params Paramètres du pipeline (commandes, pipes, pids, etc.)
 * @return 0 si succès, 1 en cas d'erreur de fork
 */
int create_pipeline_processes(t_pipeline_params *params)
{
	int i;

	// Initialisation des PIDs à -1 pour chaque commande du pipeline
	initialize_pipeline_pids(params->pids, params->cmd_count);
	i = 0;
	// On parcourt le tableau des commandes et on crée un processus pour chaque commande
	while (i < params->cmd_count)
	{
		params->index = i;
		// On passe à create_child process toute la structure de paramètres
		// qui contient les commandes, pipes, pids, index, env et shell.
		// Comme on lui passe l'index, il sait quelle commande exécuter.
		if (create_child_process(params) != 0)
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
