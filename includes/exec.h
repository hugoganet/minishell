/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 09:15:00 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/04 17:57:07 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
#define EXEC_H

#include "minishell.h"

// !===========================================================================
// !                              EXEC_CORE.C                                =
// !===========================================================================

int exec_cmd(t_ast *cmd_node, t_env *env, t_ast *ast_root, t_shell *shell);

// !===========================================================================
// !                            EXEC_COMMAND.C                               =
// !===========================================================================

t_ast *find_cmd_node(t_ast *node);
int execute_fork_process(t_ast *cmd_node, t_env *env, t_ast *ast_root, t_shell *shell);

// !===========================================================================
// !                             EXEC_ARGS.C                                 =
// !===========================================================================

void filter_empty_args(char **args);
int validate_command(t_ast *cmd_node);

// !===========================================================================
// !                             EXEC_FREE.C                                 =
// !===========================================================================

void free_child_exec(char *path, char **envp, t_shell *shell);
void free_string_array(char **arr);

// !===========================================================================
// !                             EXEC_UTILS.C                                =
// !===========================================================================

void reset_signals_in_child(void);
void print_command_not_found_error(char *cmd_name);
int is_directory(char *path);

// !===========================================================================
// !                           PATH_RESOLUTION.C                             =
// !===========================================================================

char *resolve_command_path(char *cmd_name, t_env *env);

// !===========================================================================
// !                           PIPE_EXECUTION.C                              =
// !===========================================================================

int execute_pipe_node(t_ast *node, t_env *env, t_shell *shell);

// !===========================================================================
// !                            REDIRECTION.C                                =
// !===========================================================================

int apply_parent_redirections(t_ast *node, t_shell *shell);
int setup_redirections(t_ast *node);

// !===========================================================================
// !                            HEREDOC_CORE.C                               =
// !===========================================================================

void setup_heredoc_redirection(t_shell *shell);
int process_heredocs(t_ast *ast_root, t_shell *shell);
int handle_heredoc(char *token_str, t_shell *shell);

// !===========================================================================
// !                            HEREDOC_UTILS.C                              =
// !===========================================================================

int init_heredoc_pipe(int pipefd[2]);
int validate_heredoc_token(char *token_str, int pipefd[2]);
int is_delimiter_line(char *line, char *delimiter_clean);
void close_pipe_fds(int pipefd[2]);
int is_heredoc_delimiter_quoted(const char *delimiter);

// !===========================================================================
// !                          HEREDOC_EXPANSION.C                            =
// !===========================================================================

char *expand_heredoc_line(char *line, int expand_enabled, t_shell *shell);
char *clean_heredoc_delimiter(const char *delimiter);
char *expand_and_clean_delimiter(const char *delimiter, t_shell *shell);

// !===========================================================================
// !                           HEREDOC_SIGNALS.C                             =
// !===========================================================================

void set_heredoc_sigint(struct sigaction *sa_old);
void restore_sigint(const struct sigaction *sa_old);

#endif
