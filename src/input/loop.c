/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 13:16:41 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/07 09:03:41 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Boucle principale du shell : lit les entrées, les traite,
 *        et quitte proprement sur Ctrl+D.
 *
 * @param shell Pointeur vers la structure du shell.
 */
void shell_loop(t_shell *shell)
{
	char *input;

	while (1)
	{
		// Affiche le prompt et lit l'entrée utilisateur
		input = prompt_readline();
		if (!input)
		{
			// Si l'entrée est NULL (Ctrl+D) ou erreur de lecture on sort du loop
			write(1, "exit\n", 5);
			cleanup_shell(shell);
			exit(0);
		}

		// Vérifier si SIGINT a été reçu pendant readline
		if (g_signal == SIGINT)
		{
			shell->last_exit_status = 130;
			g_signal = 0;
		}

		// Si l'entrée est vide (que des espaces), on ne traite pas l'entrée.
		// Si l'entrée contient des quotes vides ("" ou ''), on la traite comme une commande.
		// Sinon, on traite l'entrée normalement.
		if (!is_line_empty(input))
		{
			if (!is_syntax_valid(input, shell))
				process_input(input, shell);
		}
		free(input);
		// On nettoie les ressources allouées par le shell après chaque entrée.
		free_ast(shell->ast);
		shell->ast = NULL;
		free_token_list(shell->tokens);
		shell->tokens = NULL;
		// Pas de cleanup_shell() ici car on garde l'env entre les commandes
	}
}
