/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_hooks.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bernard <bernard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 17:24:56 by hugoganet         #+#    #+#             */
/*   Updated: 2025/06/03 15:31:28 by bernard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>

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

