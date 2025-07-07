/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 17:38:44 by elaudrez          #+#    #+#             */
/*   Updated: 2025/07/07 17:34:18 by hugoganet        ###   ########.fr       */
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
#if defined(__linux__)
#include <linux/limits.h>
#elif defined(__APPLE__)
#include <limits.h>
#else
#include <limits.h>
#endif
#include "libft.h"
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "ast.h"

typedef struct s_heredoc_fd
{
	int					fd;
	struct s_heredoc_fd	*next;
}	t_heredoc_fd;

// ! ----------------------- VAR GLOBALE --------------
/**
 * @brief Variable globale pour gérer les signaux.
 * 
 * Utilisée pour indiquer si un signal a été reçu, par exemple SIGINT (Ctrl+C).
 * Elle est déclarée comme volatile pour éviter les optimisations du compilateur
 * qui pourraient la rendre inaccessible dans les gestionnaires de signaux.
 * Elle est de type sig_atomic_t pour garantir que les opérations
 * sur cette variable sont atomiques,
 * c'est-à-dire qu'elles ne seront pas interrompues
 * par un signal en cours de traitement.
 */
extern volatile sig_atomic_t g_signal;

// ! ----------------------- STRUCTURES --------------

typedef struct s_env t_env;

typedef struct s_shell
{
	char **env;
	t_env *env_list;
	t_env *export_list;
	t_token *tokens;
	t_ast *ast;
	int last_exit_status;
	t_heredoc_fd *heredoc_fds; // Liste chaînée des fd heredoc
} t_shell;

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

// Définition des couleurs ANSI pour l'affiche de l'AST
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
void shell_loop(t_shell *shell);
void process_input(char *input, t_shell *shell);
void free_token_list(t_token *head);
void print_token_list(t_token *tokens, char *title);
t_env *init_env_list(char **envp);
int execute_ast(t_ast *node, t_env *env, t_shell *shell);
void free_split(char **split);
char *get_env_value(t_env *env, const char *key);
char **env_to_char_array(t_env *env);
int ft_strcmp(char *s1, const char *s2);
t_env *init_env_list(char **envp);
int execute_ast(t_ast *node, t_env *env, t_shell *shell);
void free_split(char **split);
char *get_env_value(t_env *env, const char *key);
char **env_to_char_array(t_env *env);
int ft_strcmp(char *s1, const char *s2);
void free_ast(t_ast *node);
void free_env_list(t_env *env);
void cleanup_shell(t_shell *shell);
void print_env_list(t_env *env);
t_env *create_env_pair(const char *key, const char *value);
int builtin_exec(t_ast *node, t_shell *data);
int is_builtin(t_ast *node);
int ft_cd(t_ast *node, t_shell *data);
int ft_echo(t_ast *node);
int ft_env(t_ast *node, t_shell *data);
int ft_pwd();
int ft_unset(t_ast *node, t_shell *data);
int ft_exit(t_ast *node, t_shell *data);
int ft_export(t_ast *node, t_shell *data);
int increment_shlvl(t_env *env_list);
int ft_is_valid(char *args);
void sort_list(t_env **export_list);
void add_new_node(t_env *new_node, t_env **env);
void create_add_new_node(char *key, char *value, t_env **env);
void print_export_list(t_env *export_list);
void ft_swap(t_env *i, t_env *j);
void sort_list(t_env **export_list);
void	free_args_on_error(char **args, int index);
int	parse_sign(char *str, int *sign);
int	will_overflow_negative(long long result, int digit);
int	handle_invalid_export(char *arg);


/**
 * @brief Ajoute un fd de heredoc à la liste chaînée du shell.
 *
 * @param shell Pointeur vers la structure principale du shell
 * @param fd    Descripteur de fichier à ajouter
 */
void add_heredoc_fd(t_shell *shell, int fd);

/**
 * @brief Ferme tous les descripteurs de heredoc présents dans la liste du shell.
 *
 * @param shell Pointeur vers la structure principale du shell
 */
void close_all_heredoc_fds(t_shell *shell);

/**
 * @brief Libère la mémoire de la liste chaînée des fd heredoc du shell.
 *
 * @param shell Pointeur vers la structure principale du shell
 */
void free_all_heredoc_fds(t_shell *shell);

// ! ----------------------- SIGNALS --------------------------

#include "expansion.h"
#include "exec.h"
#include "syntax.h"

#endif