/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 10:45:00 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/06 10:51:13 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_H
#define AST_H

#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include "libft.h"

typedef struct s_env t_env;
typedef struct s_shell t_shell;

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

typedef struct s_redir
{
	t_token *type;
	char *file;
	struct s_redir *next;
} t_redir;

typedef struct s_ast
{
	t_token_type type;
	char *str;
	t_redir *redir;
	struct s_ast *left;
	struct s_ast *right;
	char **args;
	int fd_in;
	int fd_out;
} t_ast;

// !===========================================================================
// !                             AST_BUILDER.C                               =
// !===========================================================================

void fill_args(t_token *node, t_ast *new_ast);
t_ast *cmd_new_ast_node(t_token *node);
t_ast *new_ast_node(t_token *node);

// !===========================================================================
// !                             AST_UTILS.C                                 =
// !===========================================================================

int token_priority(t_token_type type);
t_token *token_to_split(t_token *node, t_token *end);
t_ast *spliter(t_token *node, t_token *end);
t_ast *build_ast(t_token *node);

// !===========================================================================
// !                             AST_EXECUTOR.C                              =
// !===========================================================================

int execute_ast(t_ast *node, t_env *env, t_shell *shell);

// !===========================================================================
// !                             UTILITY FUNCTIONS                           =
// !===========================================================================

void free_ast(t_ast *node);
void pretty_print_ast(t_ast *node, int depth, const char *label);
void print_ast_cmd_node(char **argv);
bool is_redirection(t_token_type type);
const char *token_color(t_token_type type);
const char *token_type_str(t_token_type type);

#endif
