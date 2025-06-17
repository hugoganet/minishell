/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 13:16:41 by hugoganet         #+#    #+#             */
/*   Updated: 2025/06/17 11:53:31 by hugoganet        ###   ########.fr       */
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
		// Si l'entrée est vide, ou si il y a une erreur de syntaxe, on ne traite pas
		// l'entrée et on continue à la boucle.
		// Sinon, on traite l'entrée.
		if (!is_line_empty(input) && !is_syntax_valid(input))
			process_input(input, shell);
		free(input);
		input = NULL;
		// On nettoie les ressources allouées par le shell après chaque entrée.
		// Mais on ne libère pas la liste d'environnement, car elle est
		// utilisée tout au long de la session du shell.
		free_ast(shell->ast);
		shell->ast = NULL;
		free_token_list(shell->tokens);
		shell->tokens = NULL;
	}
}
