/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_complex.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 16:00:00 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/09 21:35:59 by elaudrez         ###   ########.fr       */
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
static int	execute_pipeline_setup(t_ast *node, t_pipeline_context *ctx,
								t_shell *shell)
{
	int	setup_result;

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
 * Prépare les paramètres, fork chaque commande, gère les erreurs et
 * nettoie si besoin.
 *
 * @param node Racine de l'AST du pipeline
 * @param data Structure contenant les commandes, pipes, pids
 * @param env Liste chaînée des variables d'environnement
 * @param shell Structure principale du shell
 * @return 0 si succès, 1 en cas d'erreur
 */
static int	execute_pipeline_process(t_ast *node, t_pipeline_context *ctx,
									t_env *env, t_shell *shell)
{
	int	setup_result;

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
int	execute_complex_pipeline(t_ast *node, t_env *env, t_shell *shell)
{
	t_pipeline_context	ctx;
	int					setup_result;

	setup_result = execute_pipeline_setup(node, &ctx, shell);
	if (setup_result != 0)
		return (setup_result);
	setup_result = execute_pipeline_process(node, &ctx, env, shell);
	if (setup_result != 0)
		return (setup_result);
	close_parent_pipes(ctx.pipes, ctx.cmd_count);
	setup_result = wait_for_all_processes(ctx.pids, ctx.cmd_count);
	init_signals();
	close_all_heredoc_fds(shell);
	free_all_heredoc_fds(shell);
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
static int	setup_pipeline_resources(t_ast *node, t_pipeline_context *ctx)
{
	int	cmd_count;

	cmd_count = count_pipeline_commands(node);
	if (cmd_count <= 0)
		return (1);
	if (allocate_pipeline_resources(cmd_count, &ctx->commands,
			&ctx->pipes, &ctx->pids) != 0)
		return (1);
	ctx->pipes_created = create_all_pipes(ctx->pipes, cmd_count);
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
int	setup_pipeline_execution(t_ast *node, t_pipeline_context *ctx,
							t_shell *shell)
{
	int	index;
	int	cmd_count;

	if (setup_pipeline_resources(node, ctx) != 0)
		return (1);
	index = 0;
	cmd_count = count_pipeline_commands(node);
	extract_pipeline_commands(node, ctx->commands, &index);
	if (process_all_heredocs(ctx->commands, cmd_count, shell) == 130)
	{
		cleanup_pipeline_resources(ctx);
		return (130);
	}
	return (0);
}
