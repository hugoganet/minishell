/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_simple.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:03:03 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/09 17:16:39 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipe.h"

/**
 * @brief Exécute un pipe simple (2 commandes seulement).
 *
 * Traite d'abord tous les heredocs au niveau parent,
 * puis crée un pipe, fork deux processus (gauche et droit),
 * connecte leur stdin/stdout via le pipe, puis attend leur fin.
 *
 * @param node Le nœud de type PIPE
 * @param env Liste chaînée des variables d'environnement
 * @param shell Structure principale du shell
 * @return Code de retour du processus droit (dernière commande du pipe)
 */
int execute_simple_pipe(t_ast *node, t_env *env, t_shell *shell)
{
	int fd[2];
	int final_status;
	int heredoc_result;

	// On initialise le statut final à 1 par défaut,
	// pour gérer les erreurs de pipe ou de fork.
	final_status = 1;
	// On traite les heredocs avant de créer le pipe et on conserve les fd de lecture
	// dans la structure heredoc_fds.
	heredoc_result = process_simple_pipe_heredocs(node, shell);
	if (heredoc_result != 0)
		return (heredoc_result);
	if (pipe(fd) == -1)
	{
		perror("minishell: pipe");
		return (1);
	}
	final_status = execute_simple_pipe_processes(fd, node, env, shell);
	close_all_heredoc_fds(shell);
	free_all_heredoc_fds(shell);
	return (final_status);
}

/**
 * @brief Traite les heredocs pour un pipe simple.
 *
 * Appelle process_heredocs sur les sous-nœuds gauche et droit du pipe.
 *
 * @param node Le nœud de type PIPE
 * @param shell Structure principale du shell
 * @return 0 en cas de succès, 130 si interruption utilisateur
 */
int process_simple_pipe_heredocs(t_ast *node, t_shell *shell)
{
	int heredoc_status;

	// On traite les heredocs dans les sous-nœuds gauche du pipe
	heredoc_status = process_heredocs(node->left, shell);
	if (heredoc_status == 130)
		return (130);
	heredoc_status = process_heredocs(node->right, shell);
	if (heredoc_status == 130)
		return (130);
	return (0);
}

/**
 * @brief Exécute les deux processus du pipe simple (gauche et droit).
 *
 * Gère le fork, la configuration des signaux, la fermeture des descripteurs,
 * et attend la fin des deux processus enfants.
 *
 * @param fd Tableau contenant les descripteurs du pipe
 * @param node Le nœud de type PIPE
 * @param env Liste chaînée des variables d'environnement
 * @param shell Structure principale du shell
 * @return Code de retour du processus droit (dernière commande du pipe)
 */
int execute_simple_pipe_processes(int fd[2], t_ast *node, t_env *env,
								  t_shell *shell)
{
	pid_t left_pid;
	pid_t right_pid;
	int final_status;
	t_simple_pipe_ctx left_ctx;
	t_simple_pipe_ctx right_ctx;

	// On initialise les variables pour les processus gauche et droit,
	// ainsi que les signaux pour ignorer les interruptions dans le parent.
	left_pid = -1;
	right_pid = -1;
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	// On crée le contexte pour les processus gauche et droit.
	// Cette syntax s'appelle "struct initialization" en C,
	// elle permet de créer et initialiser une structure en une seule ligne.
	left_ctx = (t_simple_pipe_ctx){fd, &left_pid, node, env, shell};
	right_ctx = (t_simple_pipe_ctx){fd, &right_pid, node, env, shell};
	// On exécute le processus gauche (cmd1) dans un fork.
	if (execute_left_pid(&left_ctx) != 0)
	{
		// SI le fork a échoué, on ferme les descripteurs de pipe,
		// on réinitialise les signaux et on retourne une erreur.
		close_pipe_fds(fd);
		init_signals();
		return (1);
	}
	// On exécute le processus droit (cmd2) dans un fork.
	if (execute_right_pid(&right_ctx) != 0)
		return (handle_pipe_execution_error(fd, left_pid));
	close_pipe_fds(fd);
	final_status = wait_for_children(left_pid, right_pid);
	init_signals();
	return (final_status);
}

/**
 * @brief Exécute le processus gauche (cmd1) dans un fork.
 *
 * Cette fonction crée un processus enfant pour exécuter la commande
 * située à gauche du pipe. Elle configure l'environnement et les redirections.
 *
 * @param ctx Contexte contenant les descripteurs de pipe, l'ID du processus,
 * et le nœud AST du pipe.
 * @return 0 si le fork réussit, 1 en cas d'erreur.
 */
int execute_left_pid(t_simple_pipe_ctx *ctx)
{
	// On passe le pid du fork (enfant) dans le contexte,
	// pour que le parent puisse le récupérer.
	*(ctx->pid) = fork();
	// Si le fork échoue, on affiche une erreur et on retourne 1.
	if (*(ctx->pid) < 0)
	{
		perror("minishell: fork");
		return (1);
	}
	// Sinon, on est dans l'enfant, on execute.
	if (*(ctx->pid) == 0)
		setup_left_child_process(ctx);
	return (0);
}

/**
 * @brief Exécute le processus droit (cmd2) dans un fork.
 *
 * Cette fonction crée un processus enfant pour exécuter la commande
 * située à droite du pipe. Elle configure l'environnement et les redirections.
 *
 * @param ctx Contexte contenant les descripteurs de pipe, l'ID du processus,
 * et le nœud AST du pipe.
 * @return 0 si le fork réussit, 1 en cas d'erreur.
 */
int execute_right_pid(t_simple_pipe_ctx *ctx)
{
	*(ctx->pid) = fork();
	if (*(ctx->pid) < 0)
	{
		perror("minishell: fork");
		return (1);
	}
	if (*(ctx->pid) == 0)
		setup_right_child_process(ctx);
	return (0);
}
