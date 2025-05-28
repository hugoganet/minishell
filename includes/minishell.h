/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 17:38:44 by elaudrez          #+#    #+#             */
/*   Updated: 2025/05/28 10:39:17 by hugoganet        ###   ########.fr       */
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
# include "libft.h"

// ! ----------------------- STRUCTURES --------------

/**
 * @struct s_shell
 * @brief Structure principale pour stocker l'état du shell.
 *
 * Elle contient :
 * 
 * - env : un tableau de chaînes de caractères représentant les variables d'environnement
 * 
 * - last_exit_status : le statut de sortie de la dernière commande exécutée
 */
typedef struct s_shell
{
	char **env;
	int last_exit_status;
} 		t_shell;

/**
 * @enum e_token_type
 * @brief Représente le type d'un token dans la ligne de commande.
 */
typedef enum e_token_type
{
	TOK_WORD,		   /** Mot (commande ou argument) */
	TOK_PIPE,		   /** Pipe '|' */
	TOK_REDIR_IN,	   /** Redirection d'entrée '<' */
	TOK_REDIR_OUT,	   /** Redirection de sortie '>' */
	TOK_REDIR_APPEND,  /** Redirection en append '>>' */
	TOK_REDIR_HEREDOC, /** Heredoc '<<' */
	TOK_AND,		   /** Opérateur logique '&&' */
	TOK_OR,			   /** Opérateur logique '||' */
	TOK_PAREN_LEFT,	   /** Parenthèse ouvrante '(' */
	TOK_PAREN_RIGHT	   /** Parenthèse fermante ')' */
}	t_token_type;

/**
 * @struct s_token
 * @brief Structure représentant un token dans la ligne de commande.
 *
 * Elle contient :
 * 
 * - value : la valeur du token (chaîne de caractères)
 * 
 * - type : le type du token (e_token_type)
 * 
 * - next : pointeur vers le token suivant dans la liste chaînée
 */
typedef struct s_token
{
	char			*value; /** Valeur du token (chaîne de caractères) */
	t_token_type	type; /** Type du token (e_token_type) */
	struct s_token	*next; /** Pointeur vers le token suivant dans la liste chaînée */
} 					t_token;

// ! ----------------------- FUNCTIONS ---------------

void			init_signals(void);
void			init_shell(t_shell *shell, char **envp);
char			**copy_env(char **envp);
void			free_env(char **env);
void			handle_signal(int signo);
char			*prompt_readline(void);
int				is_line_empty(char *input);
int				has_unclosed_quotes(char *input);
int				has_invalid_pipes(char *input);
int				has_invalid_redirections(char *input);
int				has_unmatched_parentheses(char *input);
int				is_syntax_valid(char *input);
int				is_parenthesis_empty(char *input, int i);
void			update_quote_state(char *quote_state, char c);
void			shell_loop(t_shell *shell);
t_token			*tokenize(char *input);
t_token			*token_new(char *value, t_token_type type);
t_token_type	get_token_type(char *str);
void			process_input(char *input, t_shell *shell);
void			free_token_list(t_token *head);
void			print_token_list(t_token *tokens);

#endif