/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_complex.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 16:00:00 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/09 19:33:33 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipe.h"

/**
 * @brief Initialise et configure l'exécution d'un pipeline complexe.
 *
 * Alloue et initialise les structures nécessaires (commandes, pipes, pids),
 * puis traite les heredocs pour chaque commande du pipeline.
 *
 * @param node Racine de l'AST représentant le pipeline
 * @param data Structure de données du pipeline à remplir
 * @param shell Structure principale du shell
 * @return 0 si succès, 1 ou 130 en cas d'erreur ou d'interruption heredoc
 */
static int execute_pipeline_setup(t_ast *node, t_pipeline_context *ctx,
								  t_shell *shell)
{
	int setup_result;

	ctx->commands = NULL;
	ctx->pipes = NULL;
	ctx->pids = NULL;
	setup_result = setup_pipeline_execution(node, ctx, shell);
	if (setup_result != 0)
		return (setup_result);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	return (0);
}

/**
 * @brief Lance les processus enfants pour chaque commande du pipeline.
 *
 * Prépare les paramètres, fork chaque commande, gère les erreurs et nettoie si besoin.
 *
 * @param node Racine de l'AST du pipeline
 * @param data Structure contenant les commandes, pipes, pids
 * @param env Liste chaînée des variables d'environnement
 * @param shell Structure principale du shell
 * @return 0 si succès, 1 en cas d'erreur
 */
static int execute_pipeline_process(t_ast *node, t_pipeline_context *ctx,
									t_env *env, t_shell *shell)
{
	int setup_result;

	ctx->cmd_count = count_pipeline_commands(node);
	ctx->env = env;
	ctx->shell = shell;
	setup_result = create_pipeline_processes(ctx);
	if (setup_result != 0)
	{
		cleanup_pipeline_resources(ctx);
		init_signals();
		return (1);
	}
	return (0);
}

/**
 * @brief Exécute un pipeline complexe (plus de deux commandes).
 *
 * Gère l'initialisation, le fork de chaque commande, la fermeture des pipes,
 * l'attente de tous les processus, et le nettoyage des ressources.
 *
 * @param node Racine de l'AST du pipeline
 * @param env Liste chaînée des variables d'environnement
 * @param shell Structure principale du shell
 * @return Code de retour de la dernière commande du pipeline
 */
int execute_complex_pipeline(t_ast *node, t_env *env, t_shell *shell)
{
	t_pipeline_context ctx;
	int setup_result;

	// Initialisation de la structure de données du pipeline, on ouvre les pipes, on process tous les heredocs,
	// on extrait les commandes de l'AST dans le tableau ctx.commands.
	setup_result = execute_pipeline_setup(node, &ctx, shell);
	if (setup_result != 0)
		return (setup_result);
	// On exécute le processus pour chaque commande du pipeline.
	setup_result = execute_pipeline_process(node, &ctx, env, shell);
	if (setup_result != 0)
	return (setup_result);
	// On ferme les pipes dans le processus parent, car il n'en a plus besoin.
	close_parent_pipes(ctx.pipes, ctx.cmd_count);
	// On attend la fin de tous les processus enfants du pipeline,
	// et on retourne le code de retour de la dernière commande du pipeline.
	setup_result = wait_for_all_processes(ctx.pids, ctx.cmd_count);
	// On réinitialise les signaux pour le shell principal,
	// car on a terminé l'exécution du pipeline.
	init_signals();
	// On ferme tous les descripteurs de heredocs ouverts dans le shell
	close_all_heredoc_fds(shell);
	free_all_heredoc_fds(shell);
	// On nettoie la mémoire des tableaux (pipes, commands, pids),
	// mais ne ferme pas les descripteurs de pipe car ils ont déjà été fermés.
	cleanup_pipeline_memory_only(&ctx);
	return (setup_result);
}

/**
 * @brief Alloue et prépare les ressources nécessaires à un pipeline.
 *
 * Alloue les tableaux de commandes, pipes et pids, puis crée les pipes.
 * Nettoie tout en cas d'échec.
 *
 * @param node Racine de l'AST du pipeline
 * @param data Structure de données du pipeline à remplir
 * @return 0 si succès, 1 sinon
 */
static int setup_pipeline_resources(t_ast *node, t_pipeline_context *ctx)
{
	int cmd_count;

	// On compte le nombre de commandes dans l'AST.
	cmd_count = count_pipeline_commands(node);
	if (cmd_count <= 0)
		return (1);
	// On alloue les ressources nécessaires pour le pipeline dans la structure ctx
	if (allocate_pipeline_resources(cmd_count, &ctx->commands,
									&ctx->pipes, &ctx->pids) != 0)
		return (1);
	// On crée tous les pipes nécessaires pour le pipeline et on store le nombre de pipes créés.
	ctx->pipes_created = create_all_pipes(ctx->pipes, cmd_count);
	// Si le nombre de pipes créés n'est pas égal à (cmd_count - 1), c'est ne erreur dans la création des pipes,
	// on nettoie et retourne une erreur.
	if (ctx->pipes_created != cmd_count - 1)
	{
		cleanup_pipeline_resources(ctx);
		return (1);
	}
	return (0);
}

/**
 * @brief Alloue, extrait les commandes et traite les heredocs pour un pipeline.
 *
 * Appelle setup_pipeline_resources, extrait les commandes de l'AST,
 * puis traite tous les heredocs du pipeline.
 *
 * @param node Racine de l'AST du pipeline
 * @param data Structure de données du pipeline à remplir
 * @param shell Structure principale du shell
 * @return 0 si succès, 1 ou 130 en cas d'erreur ou d'interruption heredoc
 */
int setup_pipeline_execution(t_ast *node, t_pipeline_context *ctx,
							 t_shell *shell)
{
	int index;
	int cmd_count;

	// On initialise les ressources du pipeline.
	// structure de données du pipeline à remplir.
	// On alloue les ressources nécessaires pour le pipeline.
	// On crée tous les pipes nécessaires pour le pipeline. (cmd_count - 1 pipes)
	if (setup_pipeline_resources(node, ctx) != 0)
		return (1);
	index = 0;
	// On compte le nombre de commandes dans l'AST.
	cmd_count = count_pipeline_commands(node);
	// On extrait les commandes de l'AST dans le tableau ctx->commands.
	extract_pipeline_commands(node, ctx->commands, &index);
	// On process tous les heredocs de l'AST, un par un, et on stock les fds de lecture dans la structure heredoc_fds.
	if (process_all_heredocs(ctx->commands, cmd_count, shell) == 130)
	{
		// Si un heredoc a été interrompu par l'utilisateur,
		// on nettoie les ressources du pipeline et on retourne 130.
		cleanup_pipeline_resources(ctx);
		return (130);
	}
	return (0);
}
