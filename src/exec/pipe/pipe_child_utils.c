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
 * Redirige l'entrée et la sortie standard du processus selon sa position dans le pipeline,
 * puis ferme tous les descripteurs de pipes inutiles.
 *
 * @param pipes Tableau des pipes (tableaux de 2 descripteurs)
 * @param cmd_count Nombre total de commandes dans le pipeline
 * @param child_index Index du processus enfant courant
 * @param shell Structure principale du shell (pour le cleanup en cas d'erreur)
 */
void setup_child_pipes(int **pipes, int cmd_count, int child_index,
					   t_shell *shell)
{
	setup_child_stdin(pipes, child_index, shell);
	setup_child_stdout(pipes, child_index, cmd_count, shell);
	close_all_child_pipes(pipes, cmd_count);
}

/**
 * @brief Configure l'entrée standard (stdin) pour un processus enfant du pipeline.
 *
 *
 * @param pipes Tableau des pipes (chaque pipe est un tableau de 2 descripteurs)
 * @param child_index Index du processus enfant courant
 * @param shell Structure principale du shell (pour le cleanup en cas d'erreur)
 */
void setup_child_stdin(int **pipes, int child_index, t_shell *shell)
{
	/**
	 * Pour tous les processus sauf le premier (child_index > 0),
	 * on redirige leur entrée standard (STDIN) pour qu’ils lisent
	 * ce que la commande précédente a écrit dans le pipe.
	 * (pipes[child_index - 1][0] est le descripteur de lecture du pipe précédent)
	 */
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
 * @brief Configure la sortie standard (stdout) pour un processus enfant du pipeline.
 *
 * Si ce n'est pas le dernier processus, redirige stdout vers le pipe suivant.
 *
 * @param pipes Tableau des pipes
 * @param child_index Index du processus enfant courant
 * @param cmd_count Nombre total de commandes dans le pipeline
 * @param shell Structure principale du shell (pour le cleanup en cas d'erreur)
 */
void setup_child_stdout(int **pipes, int child_index, int cmd_count,
						t_shell *shell)
{
	/**
	 * Pour tous les processus sauf le dernier,
	 * on redirige leur sortie standard (STDOUT) vers le pipe suivant,
	 * pour que la commande suivante lise ce qu'ils écrivent.
	 * (pipes[child_index][1] est le descripteur d'écriture du pipe suivant)
	 */
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
void close_all_child_pipes(int **pipes, int cmd_count)
{
	int j;

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
 * Configure les signaux, les redirections heredoc, les pipes, puis exécute la commande.
 * Nettoie toutes les ressources avant de quitter le processus.
 *
 * @param ctx Contexte d'exécution du processus enfant (commandes, pipes, env, shell, etc.)
 */
void execute_pipeline_child(t_pipeline_child_ctx *ctx)
{
	t_ast *current_cmd;
	int status;

	// On est dans le processus enfant, on réinitialise les signaux à DFL
	reset_signals_in_child();
	signal(SIGPIPE, SIG_DFL);
	// On traite les redirections heredoc si nécessaire.
	// en ramplacant STDIN par le fd de lecture du dernier heredoc.
	setup_heredoc_redirection(ctx->shell);
	current_cmd = ctx->commands[ctx->index];
	// On configure les dup2 pour que chaque processus enfant lise dans le pipe précédent
	// et écrive dans le pipe suivant.
	setup_child_pipes(ctx->pipes, ctx->cmd_count, ctx->index, ctx->shell);
	// On ferme les fds des heredocs ouverts, car à ce stade ils ont été dupliqués
	// en ramplacant STDIN par le fd de lecture du dernier heredoc.
	close_all_heredoc_fds(ctx->shell);
	// On free les pipes, pids et commandes inutiles
	// pour éviter les fuites de mémoire et les erreurs de segmentation.
	cleanup_child_memory_early(ctx->commands, ctx->pipes, ctx->pids, ctx->cmd_count - 1);
	// On exécute la commande du pipeline dans le processus enfant.
	status = exec_cmd_no_heredoc(current_cmd, ctx->env, current_cmd,
								 ctx->shell);
	// On nettoie la structure shell pour libérer les ressources allouées.
	// Cela inclut la libération de l'environnement, des listes chaînées, les heredocs, etc.
	cleanup_shell(ctx->shell);
	exit(status);
}
