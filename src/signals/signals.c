/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 14:18:46 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/08 12:35:59 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <termios.h>

volatile sig_atomic_t	g_signal = 0;

/**
 * @brief Handler pour SIGINT (Ctrl-C).
 *
 * Affiche une nouvelle ligne et marque le signal reçu.
 */
static void	handle_sigint(int sig)
{
	(void)sig;
	g_signal = SIGINT;
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

/**
 * @brief Active les handlers pour le shell interactif (readline).
 * 
 * On ignore le signal SIGPIPE pour éviter les erreurs de pipe cassé
 * (par exemple, si un processus enfant est terminé avant que le parent
 * n'ait fini d'écrire dans le pipe ex : `sleep 1234 | ls`)
 */
void	init_signals(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGPIPE, SIG_IGN);
}
