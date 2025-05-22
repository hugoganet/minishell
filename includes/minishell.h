/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 17:38:44 by elaudrez          #+#    #+#             */
/*   Updated: 2025/05/22 17:35:50 by hugoganet        ###   ########.fr       */
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

// ----------------------- FUNCTIONS ---------------

void	init_signals(void);
char	**copy_env(char **envp);
void	free_env(char **env);
void	handle_signal(int signo);

#endif