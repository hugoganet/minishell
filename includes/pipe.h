/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:04:24 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/07 11:50:55 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPE_H
#define PIPE_H

#include "minishell.h"

typedef struct s_pipeline_ctx
{
	t_ast		**commands;
	int			**pipes;
	pid_t		*pids;
	int			cmd_count;
	int			index;
	t_env		*env;
	t_shell		*shell;
}	t_pipeline_ctx;

typedef struct s_pipeline_setup
{
	t_ast		*node;
	t_ast		***commands;
	int			***pipes;
	pid_t		**pids;
	int			*pipes_created;
	t_shell		*shell;
}	t_pipeline_setup;

typedef struct s_simple_pipe_ctx
{
	int			*fd;
	pid_t		*pid;
	t_ast		*node;
	t_env		*env;
	t_shell		*shell;
}	t_simple_pipe_ctx;

// Pipe simple
int execute_simple_pipe(t_ast *node, t_env *env, t_shell *shell);
int execute_left_pid(t_simple_pipe_ctx *ctx);
int execute_right_pid(t_simple_pipe_ctx *ctx);
int process_simple_pipe_heredocs(t_ast *node, t_shell *shell);
int execute_simple_pipe_processes(int fd[2], t_ast *node, t_env *env, t_shell *shell);
void setup_left_child_process(t_simple_pipe_ctx *ctx);
void setup_right_child_process(t_simple_pipe_ctx *ctx);
int wait_for_children(pid_t left_pid, pid_t right_pid);
int handle_pipe_execution_error(int fd[2], pid_t left_pid);

// Pipe complexe
int execute_complex_pipeline(t_ast *node, t_env *env, t_shell *shell);
int setup_pipeline_execution(t_ast *node, t_ast ***commands, int ***pipes, pid_t **pids, int *pipes_created, t_shell *shell);
int create_pipeline_processes(t_ast **commands, int **pipes, pid_t *pids, int cmd_count, t_env *env, t_shell *shell);
int wait_for_all_processes(pid_t *pids, int cmd_count);

// Utilitaires pipeline
int count_pipeline_commands(t_ast *node);
void extract_pipeline_commands(t_ast *node, t_ast **commands, int *index);
bool is_complex_pipeline(t_ast *node);
int allocate_pipeline_resources(int cmd_count, t_ast ***commands, int ***pipes, pid_t **pids);
int create_all_pipes(int **pipes, int cmd_count);

// Utilitaires FD pipeline
void close_parent_pipes(int **pipes, int cmd_count);

// Utilitaires enfants pipeline
void setup_child_pipes(int **pipes, int cmd_count, int child_index, t_shell *shell);
void setup_child_stdin(int **pipes, int child_index, t_shell *shell);
void setup_child_stdout(int **pipes, int child_index, int cmd_count, t_shell *shell);
void close_all_child_pipes(int **pipes, int cmd_count);
void execute_pipeline_child(t_pipeline_ctx *ctx);

// Cleanup pipeline
void cleanup_pipeline_resources(t_ast **commands, int **pipes, pid_t *pids, int pipes_created);
void cleanup_pipeline_memory_only(t_ast **commands, int **pipes, pid_t *pids, int pipes_created);
void cleanup_child_memory_early(t_ast **commands, int **pipes, pid_t *pids, int pipes_created);
void terminate_child_processes(pid_t *pids, int count);
void initialize_pipeline_pids(pid_t *pids, int cmd_count);

// Heredoc pipeline
int process_all_heredocs(t_ast **commands, int cmd_count, t_shell *shell);

// Statut pipeline
int handle_process_exit_status(int status);

// Fonction principale
int execute_pipe_node(t_ast *node, t_env *env, t_shell *shell);

#endif