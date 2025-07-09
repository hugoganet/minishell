/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_status.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 16:00:00 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/09 18:55:45 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipe.h"

/**
 * @brief Analyse et retourne le code de sortie d'un processus enfant du pipeline.
 *
 * Gère les cas de sortie normale (exit), de terminaison par signal (SIGINT, SIGQUIT),
 * et affiche les messages appropriés sur la sortie standard.
 *
 * @param status Statut retourné par waitpid
 * @return Code de sortie à utiliser pour le shell (0-255 ou 128+signal)
 */
int handle_process_exit_status(int status)
{
	// Si le processus s'est terminé normalement (exit ou return),
	// On retourne le code de sortie
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	// Si le processus a été tué par un signal (ex: Ctrl+C ou Ctrl+\)
	if (WIFSIGNALED(status))
	{
		// Si c'est un SIGINT (Ctrl+C), on affiche juste un retour à la ligne
		if (WTERMSIG(status) == SIGINT)
			write(STDOUT_FILENO, "\n", 1);
		// Si c'est un SIGQUIT (Ctrl+\), on affiche un message spécifique
		else if (WTERMSIG(status) == SIGQUIT)
			write(STDOUT_FILENO, "Quit (core dumped)\n", 20);
		// On retourne 128 + numéro du signal, convention shell
		return (128 + WTERMSIG(status));
	}
	// Cas inattendu : on retourne 1 (erreur générique)
	return (1);
}
