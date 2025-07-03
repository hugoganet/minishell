/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_signal_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 09:51:52 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/03 10:22:37 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void heredoc_sigint(int signo)
{
	(void)signo;
	g_signal = SIGINT;
}

/**
 * @brief Installe un handler SIGINT temporaire pour le heredoc.
 *
 * @param old Pointeur vers la structure pour sauvegarder l'ancien handler
 */
void set_heredoc_sigint(struct sigaction *old)
{
	struct sigaction sa_new;
	sa_new.sa_handler = heredoc_sigint;
	sigemptyset(&sa_new.sa_mask);
	sa_new.sa_flags = 0;
	sigaction(SIGINT, &sa_new, old);
}

/**
 * @brief Restaure le handler SIGINT précédent.
 *
 * @param old Pointeur vers la structure contenant l'ancien handler
 */
void restore_sigint(const struct sigaction *old)
{
	sigaction(SIGINT, old, NULL);
}