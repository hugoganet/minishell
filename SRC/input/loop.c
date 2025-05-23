/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 13:16:41 by hugoganet         #+#    #+#             */
/*   Updated: 2025/05/23 17:53:38 by hugoganet        ###   ########.fr       */
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

	shell->last_exit_status = 0; // À supprimer après, c'est juste pour -Werror
	while (1)
	{
		input = prompt_readline(); // Affiche le prompt et lit l'entrée utilisateur
		if (!input) // Si l'entrée est NULL (Ctrl+D)
		{
			write(1, "exit\n", 5);
			break;
		}
		if (!is_line_empty(input) && !is_syntax_valid(input)) // Vérifie si la ligne n'est pas vide et si la syntaxe est valide
			process_input(input, shell); // Traite l'entrée
		free(input);
	}
}
