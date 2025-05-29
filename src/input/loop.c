/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 13:16:41 by hugoganet         #+#    #+#             */
/*   Updated: 2025/05/29 17:45:44 by hugoganet        ###   ########.fr       */
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
			break;
		}
		// Si l'entrée est vide, ou si il y a une erreur de syntaxe, on ne traite pas
		// l'entrée et on continue à la boucle.
		// Sinon, on traite l'entrée.
		if (!is_line_empty(input) && !is_syntax_valid(input)) // Vérifie si la ligne n'est pas vide et si la syntaxe est valide
			process_input(input, shell);
		// On libère la mémoire allouée pour l'entrée
		free(input);
	}
}
