/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:04:24 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/09 21:42:04 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPE_H
# define PIPE_H

# include "minishell.h"

/**
 * @struct s_pipeline_context
 * @brief Structure unifiée pour l'exécution d'un pipeline complexe
 *
 * Cette structure centralise toutes les données nécessaires pour exécuter
 * un pipeline complexe avec plusieurs commandes. Elle remplace les anciennes
 * structures s_pipeline_data et s_pipeline_params pour éliminer la redondance.
 *
 * @param commands Tableau de pointeurs vers les nœuds de commandes AST
 * @param pipes Tableau 2D des descripteurs de fichiers des pipes
 *              [index_pipe][0=lecture, 1=écriture]
 * @param pids Tableau des IDs de processus pour les processus enfants forkés
 * @param cmd_count Nombre total de commandes dans le pipeline
 * @param pipes_created Nombre de pipes créés avec succès (cmd_count-1)
 * @param current_index Index de la commande actuellement traitée
 * @param env Variables d'environnement pour l'exécution des commandes
 * @param shell État et configuration principale du shell
 */
typedef struct s_pipeline_context
{
	t_ast	**commands;
	int		**pipes;
	pid_t	*pids;
	int		cmd_count;
	int		pipes_created;
	int		current_index;
	t_env	*env;
	t_shell	*shell;
}	t_pipeline_context;

/**
 * @struct s_simple_pipe_ctx
 * @brief Structure de contexte pour l'exécution d'un pipe simple à deux
 * commandes
 *
 * Cette structure est utilisée spécifiquement pour les pipes simples
 * (cmd1 | cmd2)
 * où seulement deux commandes sont impliquées. Elle contient les
 * descripteurs
 * de fichiers du pipe, les IDs de processus, et le contexte d'exécution
 * nécessaire
 * pour les commandes côté gauche et côté droit.
 *
 * @param fd Tableau des descripteurs de fichiers du pipe
 * @param pid Tableau des IDs de processus
 *            [0=processus_gauche, 1=processus_droit]
 * @param node Pointeur vers le nœud AST du pipe contenant les deux commandes
 * @param env Variables d'environnement pour l'exécution des commandes
 * @param shell État et configuration principale du shell
 */
typedef struct s_simple_pipe_ctx
{
	int		*fd;
	pid_t	*pid;
	t_ast	*node;
	t_env	*env;
	t_shell	*shell;
}	t_simple_pipe_ctx;

// !===========================================================================
// !                           PIPE_SIMPLE.C                                 =
// !===========================================================================

int		execute_simple_pipe(t_ast *node, t_env *env, t_shell *shell);
int		execute_left_pid(t_simple_pipe_ctx *ctx);
int		execute_right_pid(t_simple_pipe_ctx *ctx);
int		process_simple_pipe_heredocs(t_ast *node, t_shell *shell);
int		execute_simple_pipe_processes(int fd[2], t_ast *node, t_env *env,
			t_shell *shell);
void	setup_left_child_process(t_simple_pipe_ctx *ctx);
void	setup_right_child_process(t_simple_pipe_ctx *ctx);
int		wait_for_children(pid_t left_pid, pid_t right_pid);
int		handle_pipe_execution_error(int fd[2], pid_t left_pid);

// !===========================================================================
// !                          PIPE_COMPLEX.C                                 =
// !===========================================================================

int		execute_complex_pipeline(t_ast *node, t_env *env, t_shell *shell);
int		setup_pipeline_execution(t_ast *node, t_pipeline_context *ctx,
			t_shell *shell);
int		create_pipeline_processes(t_pipeline_context *ctx);
int		wait_for_all_processes(pid_t *pids, int cmd_count);
int		create_child_process(t_pipeline_context *ctx);

// !===========================================================================
// !                         PIPELINE_UTILS.C                                =
// !===========================================================================

int		count_pipeline_commands(t_ast *node);
void	extract_pipeline_commands(t_ast *node, t_ast **commands, int *index);
bool	is_complex_pipeline(t_ast *node);
int		allocate_pipeline_resources(int cmd_count, t_ast ***commands,
			int ***pipes, pid_t **pids);
int		create_all_pipes(int **pipes, int cmd_count);

// !===========================================================================
// !                        PIPELINE_FD_UTILS.C                              =
// !===========================================================================

void	close_parent_pipes(int **pipes, int cmd_count);

// !===========================================================================
// !                      PIPELINE_CHILD_UTILS.C                             =
// !===========================================================================

void	setup_child_pipes(int **pipes, int cmd_count, int child_index,
			t_shell *shell);
void	setup_child_stdin(int **pipes, int child_index, t_shell *shell);
void	setup_child_stdout(int **pipes, int child_index, int cmd_count,
			t_shell *shell);
void	close_all_child_pipes(int **pipes, int cmd_count);
void	execute_pipeline_child(t_pipeline_context *ctx);

// !===========================================================================
// !                       PIPELINE_CLEANUP.C                                =
// !===========================================================================

void	cleanup_pipeline_resources(t_pipeline_context *ctx);
void	cleanup_pipeline_memory_only(t_pipeline_context *ctx);
void	cleanup_child_memory_early(t_ast **commands, int **pipes, pid_t *pids,
			int pipes_created);
void	terminate_child_processes(pid_t *pids, int count);
void	initialize_pipeline_pids(pid_t *pids, int cmd_count);

// !===========================================================================
// !                       PIPELINE_HEREDOC.C                                =
// !===========================================================================

int		process_all_heredocs(t_ast **commands, int cmd_count, t_shell *shell);

// !===========================================================================
// !                       PIPELINE_STATUS.C                                 =
// !===========================================================================

int		handle_process_exit_status(int status);

// !===========================================================================
// !                           PIPE_MAIN.C                                   =
// !===========================================================================

int		execute_pipe_node(t_ast *node, t_env *env, t_shell *shell);

#endif