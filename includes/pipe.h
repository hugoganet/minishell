/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:04:24 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/07 17:21:38 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPE_H
# define PIPE_H

# include "minishell.h"

/**
 * @struct s_pipeline_ctx
 * @brief Context structure for pipeline child process execution
 * 
 * This structure contains all the necessary information for a child process
 * to execute within a complex pipeline. It holds references to the command
 * array, pipe file descriptors, process IDs, and shell environment.
 * 
 * @param commands Array of AST command nodes to execute in the pipeline
 * @param pipes 2D array of pipe file descriptors [pipe_index][read/write]
 * @param pids Array of process IDs for all processes in the pipeline
 * @param cmd_count Total number of commands in the pipeline
 * @param index Current command index (position in the pipeline)
 * @param env Pointer to the environment variables list
 * @param shell Pointer to the main shell structure
 */
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

/**
 * @struct s_pipeline_data
 * @brief Main data structure for complex pipeline execution
 * 
 * This structure holds all the essential data needed for executing a complex
 * pipeline with multiple commands. It contains allocated arrays for commands,
 * pipes, and process IDs, along with a count of successfully created pipes.
 * 
 * @param commands Array of pointers to AST command nodes
 * @param pipes 2D array of pipe file descriptors [pipe_index][0=read, 1=write]
 * @param pids Array of process IDs for forked child processes
 * @param pipes_created Number of pipes successfully created (cmd_count-1)
 */
typedef struct s_pipeline_data
{
	t_ast		**commands;
	int			**pipes;
	pid_t		*pids;
	int			pipes_created;
}	t_pipeline_data;

/**
 * @struct s_pipeline_params
 * @brief Parameter structure for pipeline process creation
 * 
 * This structure encapsulates all parameters needed for creating and managing
 * pipeline processes. It's used to reduce the number of function parameters
 * when passing data between pipeline functions, improving code readability
 * and maintainability.
 * 
 * @param commands Array of AST command nodes to execute
 * @param pipes 2D array of pipe file descriptors
 * @param pids Array to store process IDs of created child processes
 * @param cmd_count Total number of commands in the pipeline
 * @param index Current command index being processed
 * @param env Environment variables for command execution
 * @param shell Main shell state and configuration
 */
typedef struct s_pipeline_params
{
	t_ast		**commands;
	int			**pipes;
	pid_t		*pids;
	int			cmd_count;
	int			index;
	t_env		*env;
	t_shell		*shell;
}	t_pipeline_params;

/**
 * @struct s_simple_pipe_ctx
 * @brief Context structure for simple two-command pipe execution
 * 
 * This structure is used specifically for simple pipes (cmd1 | cmd2) where
 * only two commands are involved. It contains the pipe file descriptors,
 * process IDs, and necessary execution context for both left and right
 * side commands.
 * 
 * @param fd Array of pipe file descriptors [0=read_end, 1=write_end]
 * @param pid Array of process IDs [0=left_process, 1=right_process]
 * @param node Pointer to the AST pipe node containing both commands
 * @param env Environment variables for command execution
 * @param shell Main shell state and configuration
 */
typedef struct s_simple_pipe_ctx
{
	int			*fd;
	pid_t		*pid;
	t_ast		*node;
	t_env		*env;
	t_shell		*shell;
}	t_simple_pipe_ctx;

// Pipe simple
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

// Pipe complexe
int		execute_complex_pipeline(t_ast *node, t_env *env, t_shell *shell);
int		setup_pipeline_execution(t_ast *node, t_pipeline_data *data,
			t_shell *shell);
int		create_pipeline_processes(t_pipeline_params *params);
int		wait_for_all_processes(pid_t *pids, int cmd_count);
void	setup_child_context(t_pipeline_ctx *ctx, t_pipeline_params *params);
int		create_child_process(t_pipeline_params *params);

// Utilitaires pipeline
int		count_pipeline_commands(t_ast *node);
void	extract_pipeline_commands(t_ast *node, t_ast **commands, int *index);
bool	is_complex_pipeline(t_ast *node);
int		allocate_pipeline_resources(int cmd_count, t_ast ***commands,
			int ***pipes, pid_t **pids);
int		create_all_pipes(int **pipes, int cmd_count);

// Utilitaires FD pipeline
void	close_parent_pipes(int **pipes, int cmd_count);

// Utilitaires enfants pipeline
void	setup_child_pipes(int **pipes, int cmd_count, int child_index,
			t_shell *shell);
void	setup_child_stdin(int **pipes, int child_index, t_shell *shell);
void	setup_child_stdout(int **pipes, int child_index, int cmd_count,
			t_shell *shell);
void	close_all_child_pipes(int **pipes, int cmd_count);
void	execute_pipeline_child(t_pipeline_ctx *ctx);

// Cleanup pipeline
void	cleanup_pipeline_resources(t_ast **commands, int **pipes, pid_t *pids,
			int pipes_created);
void	cleanup_pipeline_memory_only(t_ast **commands, int **pipes,
			pid_t *pids, int pipes_created);
void	cleanup_child_memory_early(t_ast **commands, int **pipes, pid_t *pids,
			int pipes_created);
void	terminate_child_processes(pid_t *pids, int count);
void	initialize_pipeline_pids(pid_t *pids, int cmd_count);

// Heredoc pipeline
int		process_all_heredocs(t_ast **commands, int cmd_count, t_shell *shell);

// Statut pipeline
int		handle_process_exit_status(int status);

// Fonction principale
int		execute_pipe_node(t_ast *node, t_env *env, t_shell *shell);

#endif