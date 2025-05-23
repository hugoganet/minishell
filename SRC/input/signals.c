/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 17:24:56 by hugoganet         #+#    #+#             */
/*   Updated: 2025/05/23 12:04:31 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Fonction de gestion des signaux reçus.
 *
 * - Ctrl-C : affiche un prompt vide (nouvelle ligne)
 * - Ctrl-\ : ignoré
 *
 * @param signo Le numéro du signal reçu.
 */
void handle_signal(int signo)
{
	if (signo == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (signo == SIGQUIT)
	{
		// Ignore SIGQUIT (Ctrl-\)
	}
}

/**
 * @brief Initialise les handlers de signaux pour le shell.
 */
void init_signals(void)
{
	signal(SIGINT, handle_signal);
	signal(SIGQUIT, SIG_IGN);
}

