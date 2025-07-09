/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_child_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 16:00:00 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/07 16:00:00 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipe.h"

/**
 * @brief Configure les pipes pour un processus enfant du pipeline.
 *
 * Redirige l'entrée et la sortie standard du processus selon sa position
 * dans le pipeline, puis ferme tous les descripteurs de pipes inutiles.
 *
 * @param pipes Tableau des pipes (tableaux de 2 descripteurs)
 * @param cmd_count Nombre total de commandes dans le pipeline
 * @param child_index Index du processus enfant courant
 * @param shell Structure principale du shell (pour le cleanup en cas d'erreur)
 */
void	setup_child_pipes(int **pipes, int cmd_count, int child_index,
					t_shell *shell)
{
	setup_child_stdin(pipes, child_index, shell);
	setup_child_stdout(pipes, child_index, cmd_count, shell);
	close_all_child_pipes(pipes, cmd_count);
}

/**
 * @brief Configure l'entrée standard (stdin) pour un processus enfant du
 * pipeline.
 *
 *
 * @param pipes Tableau des pipes (chaque pipe est un tableau de 2 descripteurs)
 * @param child_index Index du processus enfant courant
 * @param shell Structure principale du shell (pour le cleanup en cas d'erreur)
 */
void	setup_child_stdin(int **pipes, int child_index, t_shell *shell)
{
	if (child_index > 0)
	{
		if (dup2(pipes[child_index - 1][0], STDIN_FILENO) == -1)
		{
			perror("minishell: dup2 stdin");
			cleanup_shell(shell);
			exit(1);
		}
	}
}

/**
 * @brief Configure la sortie standard (stdout) pour un processus enfant
 * du pipeline.
 *
 * Si ce n'est pas le dernier processus, redirige stdout vers le pipe suivant.
 *
 * @param pipes Tableau des pipes
 * @param child_index Index du processus enfant courant
 * @param cmd_count Nombre total de commandes dans le pipeline
 * @param shell Structure principale du shell (pour le cleanup en cas d'erreur)
 */
void	setup_child_stdout(int **pipes, int child_index, int cmd_count,
						t_shell *shell)
{
	if (child_index < cmd_count - 1)
	{
		if (dup2(pipes[child_index][1], STDOUT_FILENO) == -1)
		{
			perror("minishell: dup2 stdout");
			cleanup_shell(shell);
			exit(1);
		}
	}
}

/**
 * @brief Ferme tous les descripteurs de pipes dans un processus enfant.
 *
 * Ferme les deux descripteurs (lecture et écriture) de chaque pipe du pipeline.
 *
 * @param pipes Tableau des pipes
 * @param cmd_count Nombre total de commandes dans le pipeline
 */
void	close_all_child_pipes(int **pipes, int cmd_count)
{
	int	j;

	j = 0;
	while (j < cmd_count - 1)
	{
		close(pipes[j][0]);
		close(pipes[j][1]);
		j++;
	}
}

/**
 * @brief Exécute le code d'un processus enfant du pipeline.
 *
 * Configure les signaux, les redirections heredoc, les pipes, puis exécute
 * la commande. Nettoie toutes les ressources avant de quitter le processus.
 *
 * @param ctx Contexte d'exécution du processus enfant (commandes, pipes, env,
 * shell, etc.)
 */
void	execute_pipeline_child(t_pipeline_context *ctx)
{
	t_ast	*current_cmd;
	int		status;

	reset_signals_in_child();
	signal(SIGPIPE, SIG_DFL);
	setup_heredoc_redirection(ctx->shell);
	current_cmd = ctx->commands[ctx->current_index];
	setup_child_pipes(ctx->pipes, ctx->cmd_count, ctx->current_index,
		ctx->shell);
	close_all_heredoc_fds(ctx->shell);
	cleanup_child_memory_early(ctx->commands, ctx->pipes, ctx->pids,
		ctx->pipes_created);
	status = exec_cmd_no_heredoc(current_cmd, ctx->env, current_cmd,
			ctx->shell);
	cleanup_shell(ctx->shell);
	exit(status);
}
