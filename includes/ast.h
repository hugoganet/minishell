/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 10:45:00 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/09 18:20:31 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_H
# define AST_H

# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>
# include "libft.h"

typedef struct s_env	t_env;
typedef struct s_shell	t_shell;

/**
 * @enum e_token_type
 * @brief Énumération représentant les différents types de tokens dans le shell.
 *
 * Cette énumération définit tous les types de tokens possibles lors de
 * l'analyse lexicale d'une ligne de commande dans le shell. Chaque type
 * correspond à un élément syntaxique spécifique qui sera traité différemment
 * lors de la construction de l'AST et de l'exécution.
 *
 * @param PIPE Token représentant l'opérateur pipe (|)
 * @param REDIR_INPUT Token représentant la redirection d'entrée (<)
 * @param REDIR_OUTPUT Token représentant la redirection de sortie (>)
 * @param REDIR_APPEND Token représentant la redirection d'ajout (>>)
 * @param HEREDOC Token représentant un heredoc (<<)
 * @param WORD Token représentant un mot générique
 * @param CMD Token représentant une commande
 * @param ARG Token représentant un argument de commande
 * @param FILES Token représentant un nom de fichier
 */
typedef enum e_token_type
{
	PIPE,
	REDIR_INPUT,
	REDIR_OUTPUT,
	REDIR_APPEND,
	HEREDOC,
	WORD,
	CMD,
	ARG,
	FILES,
}	t_token_type;

/**
 * @struct s_token
 * @brief Structure représentant un token dans la liste chaînée de tokens.
 *
 * Cette structure stocke un token individuel avec son contenu textuel,
 * son type et un pointeur vers le token suivant.
 *
 * @param str Chaîne de caractères contenant le contenu du token
 * @param type Type du token (défini par l'énumération t_token_type)
 * @param next Pointeur vers le token suivant dans la liste chaînée
 */
typedef struct s_token
{
	char			*str;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

/**
 * @struct s_redir
 * @brief Structure représentant une redirection dans une commande.
 *
 * Cette structure stocke les informations nécessaires pour une redirection,
 * incluant le type de redirection et le fichier cible.
 *
 * @param type Pointeur vers le token contenant le type de redirection
 * @param file Nom du fichier pour la redirection
 * @param next Pointeur vers la prochaine redirection (redirections multiples)
 */
typedef struct s_redir
{
	t_token			*type;
	char			*file;
	struct s_redir	*next;
}	t_redir;

/**
 * @struct s_ast
 * @brief Structure représentant un nœud de l'arbre de syntaxe abstraite (AST).
 *
 * Cette structure forme l'arbre de syntaxe abstraite qui représente
 * la structure hiérarchique des commandes et opérateurs dans le shell.
 * Chaque nœud peut représenter une commande, un pipe, ou une redirection.
 *
 * @param type Type du nœud (commande, pipe, redirection, etc.)
 * @param str Chaîne de caractères associée au nœud (nom de commande, etc.)
 * @param redir Liste chaînée des redirections associées à ce nœud
 * @param left Pointeur vers le nœud enfant gauche (pour les opérateurs binaires)
 * @param right Pointeur vers le nœud enfant droit (pour les opérateurs binaires)
 * @param args Tableau des arguments de la commande (terminé par NULL)
 * @param fd_in Descripteur de fichier d'entrée pour ce nœud
 * @param fd_out Descripteur de fichier de sortie pour ce nœud
 */
typedef struct s_ast
{
	t_token_type	type;
	char			*str;
	t_redir			*redir;
	struct s_ast	*left;
	struct s_ast	*right;
	char			**args;
	int				fd_in;
	int				fd_out;
}	t_ast;

// !===========================================================================
// !                             AST_BUILDER.C                               =
// !===========================================================================

void		fill_args(t_token *node, t_ast *new_ast);
t_ast		*cmd_new_ast_node(t_token *node);
t_ast		*new_ast_node(t_token *node);

// !===========================================================================
// !                             AST_CORE.C                                   =
// !===========================================================================

int			token_priority(t_token_type type);
t_token		*token_to_split(t_token *node, t_token *end);
t_ast		*spliter(t_token *node, t_token *end);
t_ast		*build_ast(t_token *node);

// !===========================================================================
// !                             AST_UTILS.C                                  =
// !===========================================================================

int			count_args(t_token *node);
int			is_arg(t_token *ptr, t_ast *new_ast, int *j);

// !===========================================================================
// !                             AST_EXECUTOR.C                              =
// !===========================================================================

int			execute_ast(t_ast *node, t_env *env, t_shell *shell);

// !===========================================================================
// !                             UTILITY FUNCTIONS                           =
// !===========================================================================

void		free_ast(t_ast *node);
void		pretty_print_ast(t_ast *node, int depth, const char *label);
void		print_ast_cmd_node(char **argv);
bool		is_redirection(t_token_type type);
const char	*token_color(t_token_type type);
const char	*token_type_str(t_token_type type);
void		free_args_on_error(char **args, int index);

#endif