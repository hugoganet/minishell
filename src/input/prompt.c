/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 13:01:41 by hugoganet         #+#    #+#             */
/*   Updated: 2025/06/18 17:03:55 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Affiche le prompt et lit l'entrée utilisateur.
 * Gère SIGINT en renvoyant une ligne vide si besoin.
 * On ne gère pas SIGQUIT ici car il faut l'ignorer dans le processus readline.
 *
 * @return La ligne lue (à free par l'appelant) ou NULL si Ctrl-D.
 */
char *prompt_readline(void)
{
	char *input;

	// Utiliser get_next_line si le terminal n'est pas interactif
	// c'est-à-dire si l'entrée standard n'est pas un terminal. (pour le testeur)
	if (!isatty(STDIN_FILENO))
	{
		// Mode non interactif : lecture depuis stdin avec get_next_line
		input = get_next_line(STDIN_FILENO);
		return (input);
	}
	// Affiche le prompt
	input = readline("minishell> ");
	// Si le signal est SIGINT (Ctrl+C)
	if (g_signal == SIGINT)
	{
		// on libère l'entrée
		free(input);
		// input = NULL;
		// Réinitialise le signal
		g_signal = 0;
		// ligne vide pour éviter d'interrompre la boucle
		return (ft_strdup(""));
	}
	// Si l'entrée n'est pas NULL et n'est pas vide
	if (input && *input)
		// Ajoute l'entrée à l'historique
		add_history(input);
	return (input);
}