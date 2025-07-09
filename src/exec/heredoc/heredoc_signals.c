/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_signals.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 09:51:52 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/09 18:34:41 by elaudrez         ###   ########.fr       */
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
void	heredoc_sigint(int signo)
{
	(void)signo;
	g_signal = SIGINT;
}

/**
 * @brief Configure le gestionnaire de signal SIGINT pour les heredocs.
 *
 * sa_handler : Pointeur vers la fonction à appeler lors de la réception
 * du signal SIGINT.
 */
void	set_heredoc_sigint(struct sigaction *old)
{
	struct sigaction	sa_new;

	sa_new.sa_handler = heredoc_sigint;
	sigemptyset(&sa_new.sa_mask);
	sa_new.sa_flags = 0;
	sigaction(SIGINT, &sa_new, old);
}

/**
 * @brief Restaure le gestionnaire de signal SIGINT précédemment configuré.
 */
void	restore_sigint(const struct sigaction *old)
{
	sigaction(SIGINT, old, NULL);
}
