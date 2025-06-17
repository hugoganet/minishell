/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 14:18:46 by hugoganet         #+#    #+#             */
/*   Updated: 2025/06/17 16:01:08 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Variable globale utilisée pour signaler un signal reçu
 *
 * Cette variable est définie ici (dans signals.c) et déclarée
 * en externe dans minishell.h pour respecter les règles du projet.
 */
volatile sig_atomic_t g_signal = 0;

/**
 * @brief Handler de signal : stocke simplement le numéro du signal.
 *
 * Ce handler est ultra simple car appelé pendant des appels système
 * non réentrants (ex: readline). Il ne doit pas appeler de fonction
 * complexe, juste stocker un signal pour traitement différé.
 *
 * @param signo Le numéro du signal reçu (ex : SIGINT)
 */
void handle_signal(int signo)
{
	g_signal = signo;
}

/**
 * @brief Initialise les handlers de signaux pour SIGINT et SIGQUIT.
 *
 * Utilise sigaction() au lieu de signal() pour une gestion propre et POSIX-safe.
 * SA_RESTART permet à readline() de continuer après interruption.
 */
void init_signals(void)
{
	struct sigaction sa;

	// On définit la fonction à appeler quand un signal est reçu
	sa.sa_handler = handle_signal;
	// On initialise le masque de signaux bloqués pendant le handler
	// (ici, on ne bloque rien en plus)
	sigemptyset(&sa.sa_mask);
	// Option SA_RESTART : certaines fonctions système (ex: readline) sont automatiquement reprises
	// après qu'un signal ait été traité (au lieu de renvoyer une erreur)
	sa.sa_flags = SA_RESTART;
	// Appliquer ce handler à SIGINT (Ctrl-C)
	if (sigaction(SIGINT, &sa, NULL) == -1)
		write(2, "Failed to set SIGINT handler\n", 30);
	// Appliquer ce handler à SIGQUIT (Ctrl-\) même s’il ne fait rien de visible
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
		write(2, "Failed to set SIGQUIT handler\n", 31);
}