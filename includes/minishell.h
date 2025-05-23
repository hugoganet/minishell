/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 17:38:44 by elaudrez          #+#    #+#             */
/*   Updated: 2025/05/23 15:51:25 by hugoganet        ###   ########.fr       */
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
 * @var s_shell::env
 * Copie locale de l'environnement (modifiable).
 * @var s_shell::last_exit_status
 * Code de sortie de la dernière commande exécutée.
 */
typedef struct s_shell
{
	char **env;
	int last_exit_status;
} t_shell;

// ! ----------------------- FUNCTIONS ---------------

void	init_signals(void);
void	init_shell(t_shell *shell, char **envp);
char	**copy_env(char **envp);
void	free_env(char **env);
void	handle_signal(int signo);
char	*prompt_readline(void);
int		is_line_empty(char *input);
int		has_unclosed_quotes(char *input);
int		has_invalid_pipes(char *input);
int		has_invalid_redirections(char *input);
void	update_quote_state(char *quote_state, char c);
void	shell_loop(t_shell *shell);


#endif