/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 17:38:44 by elaudrez          #+#    #+#             */
/*   Updated: 2025/06/04 18:11:14 by hugoganet        ###   ########.fr       */
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
#include "libft.h"
#include <sys/wait.h>

// ! ----------------------- STRUCTURES --------------

typedef struct s_env t_env;

/**
 * @struct s_shell
 * @brief Structure principale pour stocker l'état du shell.
 *
 * Elle contient :
 *
 * - `env`: un tableau de chaînes de caractères représentant les variables d'environnement.
 *
 * - `last_exit_status`: le statut de sortie de la dernière commande exécutée.
 */
typedef struct s_shell
{
	char **env;
	t_env *env_list;
	int last_exit_status;
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

/**
 * @struct s_token
 * @brief Structure représentant un token dans la ligne de commande.
 *
 * Chaque token a :
 *
 * - `str`: la chaîne de caractères du token.
 *
 * - `type`: le type du token (défini par l'énumération `e_token_type`).
 *
 * - `next`: un pointeur vers le token suivant dans la liste chaînée.
 */
typedef struct s_token
{
	char *str;
	t_token_type type;
	struct s_token *next;
} t_token;


// Initialisation de la structure de l'arbre de syntaxe abstraite (AST)
typedef struct s_ast t_ast;

	/**
	 * @struct s_ast
	 * @brief Représente un nœud de l'arbre de syntaxe abstraite (AST).
	 *
	 * Chaque nœud a :
	 *
	 * - `type`: le type du nœud (défini par l'énumération `e_token_type`).
	 *
	 * - `str`: la chaîne de caractères associée au nœud.
	 *
	 * - `left`: un pointeur vers le sous-arbre gauche.
	 *
	 * - `right`: un pointeur vers le sous-arbre droit.
	 */
	typedef struct s_ast
{
	t_token_type type;
	char *str;
	t_ast *left;
	t_ast *right;
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
} 	t_env;

// Définition des couleurs ANSI
#define COLOR_CMD "\033[1;36m"	 // Cyan clair
#define COLOR_ARG "\033[1;34m"	 // Bleu
#define COLOR_PIPE "\033[1;32m"	 // Vert
#define COLOR_REDIR "\033[1;35m" // Magenta
#define COLOR_HEREDOC "\033[1;33m" // Jaune
#define COLOR_FILES "\033[1;31m" // Rouge
#define COLOR_RESET "\033[0m"	 // Reset

// ! ----------------------- FUNCTIONS ---------------

void init_signals(void);
void init_shell(t_shell *shell, char **envp);
char **copy_env(char **envp);
void free_env(char **env);
void handle_signal(int signo);
char *prompt_readline(void);
int is_line_empty(char *input);
int has_unclosed_quotes(char *input);
int has_invalid_pipes(char *input);
int has_invalid_redirections(char *input);
int has_unmatched_parentheses(char *input);
int is_syntax_valid(char *input);
int is_parenthesis_empty(char *input, int i);
void update_quote_state(char *quote_state, char c);
void shell_loop(t_shell *shell);
t_token *tokenize(char *input);
t_token *token_new(char *str, t_token_type type);
t_token_type get_token_type(char *str);
void process_input(char *input, t_shell *shell);
void free_token_list(t_token *head);
void print_token_list(t_token *tokens);
bool is_redirection(t_token_type type);
void refine_token_types(t_token *head);
char *parse_quoted_token(char *input, int *i);
void append_token(t_token **head, t_token **last, t_token *new);
int validate_token_sequence(t_token *head);
t_env *init_env_list(char **envp);
t_ast *build_ast(t_token *node);
void expand_vars(t_ast *node, t_shell *data);
char *ft_strcpy(char *dest, char *src);
int which_quote(t_ast *node);
void pretty_print_ast(t_ast *node, int depth);
const char *token_type_str(t_token_type type);
const char *token_color(t_token_type type);
int execute_ast(t_ast *node, t_env *env);
int exec_cmd(t_ast *cmd_node, t_env *env);
void print_ast_cmd_node(char **argv);
void free_split(char **split);
char *get_env_value(t_env *env, const char *key);
char **env_to_char_array(t_env *env);
char *resolve_command_path(char *cmd_name, t_env *env);
int ft_strcmp(char *s1, const char *s2);

#endif