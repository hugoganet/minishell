/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_signals.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 09:51:52 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/09 14:57:39 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"

/**
 * @brief Gestionnaire de signal pour SIGINT (Ctrl+C) pendant les heredocs.
 * 
 * Cette fonction est appelée lorsque le signal SIGINT est reçu
 * pendant l'exécution d'un heredoc et marque le signal global
 * g_signal à SIGINT.
 */
void heredoc_sigint(int signo)
{
	(void)signo;
	g_signal = SIGINT;
}

/**
 * @brief Configure le gestionnaire de signal SIGINT pour les heredocs.
 *
 * sa_handler : Pointeur vers la fonction à appeler lors de la réception du signal SIGINT.
 */
void set_heredoc_sigint(struct sigaction *old)
{
	struct sigaction sa_new;

	// Définition du nouveau gestionnaire de signal pour SIGINT (Ctrl+C)
	// sa_handler : Pointeur vers la fonction à appeler lors de la réception du signal SIGINT.
	// Ici, heredoc_sigint est appelée pour gérer l'interruption lors d'un heredoc.
	// SIGINT est la macro standard pour le signal d'interruption (Ctrl+C).
	// Plus ou moins égal à signal(SIGINT, heredoc_sigint);
	sa_new.sa_handler = heredoc_sigint;
	// sigemptyset initialise sa_new.sa_mask (masque de signaux) à vide (aucun signal bloqué pendant l'exécution du handler)
	sigemptyset(&sa_new.sa_mask);
	// sa_flags permet de spécifier des options pour le comportement du handler (ici aucune option)
	sa_new.sa_flags = 0;

	// sigaction configure le nouveau gestionnaire pour SIGINT et sauvegarde l'ancien dans *old
	sigaction(SIGINT, &sa_new, old);
}

/**
 * @brief Restaure le gestionnaire de signal SIGINT précédemment configuré.
 */
void restore_sigint(const struct sigaction *old)
{
	sigaction(SIGINT, old, NULL);
}
