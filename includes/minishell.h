/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 17:38:44 by elaudrez          #+#    #+#             */
/*   Updated: 2025/07/03 09:33:59 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <stdbool.h>
#include <linux/limits.h> // ! raised an error on macOs, check .vscode/settings.json : "C_Cpp.errorSquiggles": "disabled"
#include "libft.h"
#include <sys/wait.h>
#include <fcntl.h>

// ! ----------------------- VAR GLOBALE --------------

// extern : la variable est déclarée ici, mais définie ailleurs (dans signals.c).
//          Ça évite de la redéfinir dans chaque fichier, tout en permettant de l'utiliser.

// volatile : indique au compilateur que cette variable peut changer à tout moment,
//            par exemple via un signal. Il doit toujours la relire depuis la mémoire.

// sig_atomic_t : type sûr pour signaux. Assure que la lecture/écriture est atomique
//                (pas interrompue en plein milieu), donc sans comportement indéfini.

extern volatile sig_atomic_t g_signal;

// ! ----------------------- STRUCTURES --------------

typedef struct s_env t_env;
typedef struct s_ast t_ast;
typedef struct s_token t_token;

typedef struct s_shell
{
	char **env;
	t_env *env_list;
	t_env *export_list;
	t_token *tokens;
	t_ast *ast;
	int last_exit_status;
	int heredoc_fd;
} t_shell;

/**
 * @enum e_token_type
 * @brief Représente le type d'un token dans la ligne de commande.
 */
typedef enum e_token_type
{
	PIPE,		  /**< Pipe '|' */
	REDIR_INPUT,  /**< Redirection d'entrée '<' */
	REDIR_OUTPUT, /**< Redirection de sortie '>' */
	REDIR_APPEND, /**< Redirection en append '>>' */
	HEREDOC,	  /**< Heredoc '<<' */
	WORD,		  /**< Mot (commande ou argument) */
	CMD,		  /**< Commande (premier mot d'une ligne) */
	ARG,		  /**< Argument (mot après la commande) */
	FILES,		  /**< Fichiers (après une redirection) */
} t_token_type;

typedef struct s_token
{
	char *str;
	t_token_type type;
	struct s_token *next;
} t_token;

typedef struct s_redir t_redir;

typedef struct s_redir
{
	t_token *type;
	char *file;
	t_redir *next;
} t_redir;

typedef struct s_ast t_ast;

typedef struct s_ast
{
	t_token_type type;
	char *str;
	t_redir *redir;
	t_ast *left;
	t_ast *right;
	char **args; // fonction compte nb d'arg apres et malloc char **
	int fd_in;
	int fd_out;
} t_ast;

/**
 * @struct s_env
 * @brief Représente une variable d'environnement sous forme de liste chaînée.
 *
 * - `key` : nom de la variable d'environnement.
 *
 * - `value` : valeur de la variable d'environnement.
 *
 * - `next` : pointeur vers l'élément suivant de la liste.
 */
typedef struct s_env
{
	char *key;
	char *value;
	struct s_env *next;
} t_env;

/**
 * @struct s_expand_ctx
 * @brief Contexte pour l'expansion des variables d'environnement.
 *
 * - `start` : position de début de l'expansion dans la chaîne.
 *
 * - `end` : position de fin de l'expansion dans la chaîne.
 *
 * - `offset` : pointeur vers un entier pour ajuster la position après expansion.
 */
typedef struct s_expand_ctx
{
	int start;
	int end;
	int *offset;
} t_expand_ctx;

// Définition des couleurs ANSI
#define COLOR_CMD "\033[1;36m"	   // Cyan clair
#define COLOR_ARG "\033[1;34m"	   // Bleu
#define COLOR_PIPE "\033[1;32m"	   // Vert
#define COLOR_REDIR "\033[1;35m"   // Magenta
#define COLOR_HEREDOC "\033[1;33m" // Jaune
#define COLOR_FILES "\033[1;31m"   // Rouge
#define COLOR_RESET "\033[0m"	   // Reset

// ! ----------------------- FUNCTIONS ---------------

void init_signals(void);
void init_shell(t_shell *shell, char **envp, t_env *env_list);
char **copy_env(char **envp);
void free_env(char **env);
char *prompt_readline(void);
int is_line_empty(char *input);
int has_unclosed_quotes(char *input);
int has_invalid_pipes(char *input);
int has_invalid_redirections(char *input);
int has_unmatched_parentheses(char *input);
int has_unclosed_braces(char *input);
int is_syntax_valid(char *input, t_shell *shell);
int is_parenthesis_empty(char *input, int i);
void update_quote_state(char *quote_state, char c);
void shell_loop(t_shell *shell);
t_token *tokenize(char *input);
t_token *token_new(char *str, t_token_type type);
t_token_type get_token_type(char *str);
void process_input(char *input, t_shell *shell);
void free_token_list(t_token *head);
void print_token_list(t_token *tokens, char *title);
bool is_redirection(t_token_type type);
void refine_token_types(t_token *head);
char *parse_quoted_token(char *input, int *i);
void append_token(t_token **head, t_token **last, t_token *new);
int validate_token_sequence(t_token *head);
t_env *init_env_list(char **envp);
t_ast *build_ast(t_token *node);
void expand_vars(t_ast *node, t_shell *data);
char *ft_strcpy(char *dest, char *src);
void pretty_print_ast(t_ast *node, int depth, const char *label);
const char *token_type_str(t_token_type type);
const char *token_color(t_token_type type);
int execute_ast(t_ast *node, t_env *env, t_shell *shell);
int exec_cmd(t_ast *cmd_node, t_env *env, t_ast *ast_root, t_shell *shell);
t_ast *find_cmd_node(t_ast *node);
void print_ast_cmd_node(char **argv);
void free_split(char **split);
char *get_env_value(t_env *env, const char *key);
char **env_to_char_array(t_env *env);
char *resolve_command_path(char *cmd_name, t_env *env);
int ft_strcmp(char *s1, const char *s2);
void free_ast(t_ast *node);
int execute_pipe_node(t_ast *node, t_env *env, t_shell *shell);
void free_env_list(t_env *env);
void cleanup_shell(t_shell *shell);
void print_env_list(t_env *env);
int setup_redirections(t_ast *node);
t_env *create_env_pair(const char *key, const char *value);
int handle_heredoc(char *token_str, t_shell *shell);
int builtin_exec(t_ast *node, t_shell *data);
int is_builtin(t_ast *node);
int ft_cd(t_ast *node, t_shell *data);
int ft_echo(t_ast *node);
int ft_env(t_ast *node, t_shell *data);
int ft_pwd();
int ft_unset(t_ast *node, t_shell *data);
int ft_exit(t_ast *node, t_shell *data);
int ft_export(t_ast *node, t_shell *data);
int apply_parent_redirections(t_ast *node, t_shell *shell);
int increment_shlvl(t_env *env_list);
int ft_is_valid(char *args);
bool is_token_delim(char c);
void sort_list(t_env **export_list);
void close_pipe_fds(int fds[2]);

// ! ----------------------- ENV VARS EXPANSION ---------------
// Quote management functions (centralized)
bool should_expand_at_position(const char *str, int pos);
bool is_expandable(const char *str);
bool is_in_quotes(const char *str, int pos, char *quote_type);

// Variable special cases (centralized)
bool is_positional_param(const char *name);
bool is_special_var(const char *name);
char *handle_special_cases(const char *name);
bool is_valid_var_start(char c);

// Core expansion functions
char *find_var(char *str, int *start, int *end);
char *copy_var_content(char *str, t_shell *data, int *start, int *end);
char *expand_exit_status(char *str, t_shell *data, int *start, int *end);
char *join_str(char *str, t_shell *data);
char *remove_quotes(char *str);
char *get_env_var_value(char *name, char **env);
char *get_raw_token_if_invalid(char *str, int start, int end);
char *handle_var_expansion(char *str, char *var, t_expand_ctx ctx);
char *process_next_dollar(char *str, int *offset, t_shell *data);
void expand_vars(t_ast *node, t_shell *data);
void get_name_brace(char *str, int *i, int *end, int *name_start);
void get_name(char *str, int *i, int *end, int *name_start);

// Utility functions
char *ft_strcpy(char *dest, char *src);

#endif