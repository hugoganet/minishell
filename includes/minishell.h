/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 17:38:44 by elaudrez          #+#    #+#             */
/*   Updated: 2025/05/29 17:31:21 by hugoganet        ###   ########.fr       */
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

// ! ----------------------- STRUCTURES --------------

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
	AND,		  /**< Opérateur logique '&&' */
	OR,			  /**< Opérateur logique '||' */
	PAREN_LEFT,	  /**< Parenthèse ouvrante '(' */
	PAREN_RIGHT	  /**< Parenthèse fermante ')' */
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
	char			*str;
	t_token_type	type;
	struct s_token	*next;
} t_token;

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

#endif