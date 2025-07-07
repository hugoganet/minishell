/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 17:38:44 by elaudrez          #+#    #+#             */
/*   Updated: 2025/07/07 18:55:26 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <stdbool.h>
# if defined(__linux__)
#  include <linux/limits.h>
# elif defined(__APPLE__)
#  include <limits.h>
# else
#  include <limits.h>
# endif
# include "libft.h"
# include <sys/wait.h>
# include <fcntl.h>
# include <sys/stat.h>
# include "ast.h"
# include "expansion.h"
# include "exec.h"
# include "syntax.h"

extern volatile sig_atomic_t	g_signal;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_shell
{
	char			**env;
	t_env			*env_list;
	t_env			*export_list;
	t_token			*tokens;
	t_ast			*ast;
	int				last_exit_status;
	t_heredoc_fd	*heredoc_fds;
}	t_shell;

# define COLOR_CMD "\033[1;36m"
# define COLOR_ARG "\033[1;34m"
# define COLOR_PIPE "\033[1;32m"
# define COLOR_REDIR "\033[1;35m"
# define COLOR_HEREDOC "\033[1;33m"
# define COLOR_FILES "\033[1;31m"
# define COLOR_RESET "\033[0m"

// !===========================================================================
// !                               MAIN.C                                    =
// !===========================================================================

void	init_signals(void);
void	init_shell(t_shell *shell, char **envp, t_env *env_list);
void	shell_loop(t_shell *shell);
void	process_input(char *input, t_shell *shell);
void	free_loop(char *input, t_shell *shell);
int		process_main_argv_as_input(char *input, t_shell *shell, char **argv);

// !===========================================================================
// !                              INPUT.C                                    =
// !===========================================================================

char	*prompt_readline(void);

// !===========================================================================
// !                           ENVIRONMENT.C                                 =
// !===========================================================================

char	**copy_env(char **envp);
void	free_env(char **env);
t_env	*init_env_list(char **envp);
char	*get_env_value(t_env *env, const char *key);
char	**env_to_char_array(t_env *env);
void	free_env_list(t_env *env);
void	print_env_list(t_env *env);
t_env	*create_env_pair(const char *key, const char *value);
int		increment_shlvl(t_env *env_list);

// !===========================================================================
// !                           SHLVL_UTILS.C                                 =
// !===========================================================================

int		shlvl_exists_in_envp(char **envp);
int		ensure_shlvl_is_one(t_env *env_list);
void	configure_shlvl(char **envp, t_env *env_list);

// !===========================================================================
// !                             CLEANUP.C                                   =
// !===========================================================================

void	cleanup_shell(t_shell *shell);
void	free_split(char **split);
void	free_token_list(t_token *head);
void	print_token_list(t_token *tokens, char *title);

// !===========================================================================
// !                          BUILT_IN_COMMANDS                              =
// !===========================================================================

int		builtin_exec(t_ast *node, t_shell *data);
int		is_builtin(t_ast *node);
int		ft_cd(t_ast *node, t_shell *data);
int		ft_echo(t_ast *node);
int		ft_env(t_ast *node, t_shell *data);
int		ft_pwd(void);
int		ft_unset(t_ast *node, t_shell *data);
int		ft_exit(t_ast *node, t_shell *data);
int		ft_export(t_ast *node, t_shell *data);

// !===========================================================================
// !                            UTILS.C                                      =
// !===========================================================================

int		ft_strcmp(char *s1, const char *s2);
int		ft_is_valid(char *args);
void	sort_list(t_env **export_list);
void	add_new_node(t_env *new_node, t_env **env);
void	create_add_new_node(char *key, char *value, t_env **env);
void	print_export_list(t_env *export_list);
void	ft_swap(t_env *i, t_env *j);
int		parse_sign(char *str, int *sign);
int		will_overflow_negative(long long result, int digit);
int		handle_invalid_export(char *arg);
bool	is_valid_var_start(char c);
bool	is_valid_var_char(char c);
bool	is_translated_string(const char *input, int dollar_pos);

#endif