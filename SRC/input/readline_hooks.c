/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_hooks.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 17:24:56 by hugoganet         #+#    #+#             */
/*   Updated: 2025/05/23 15:47:35 by hugoganet        ###   ########.fr       */
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
	if (signo == SIGINT) // Ctrl-C
	{
		write(1, "\n", 1);
		rl_on_new_line(); // Déplace le curseur à la nouvelle ligne
		rl_replace_line("", 0); // Remplace la ligne actuelle par une chaîne vide
		rl_redisplay(); // Redessine le prompt
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

