/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 13:16:41 by hugoganet         #+#    #+#             */
/*   Updated: 2025/06/17 16:02:22 by hugoganet        ###   ########.fr       */
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
		// Gestion des signaux : `Ctrl-C` et `Ctrl-\`
		if (g_signal == SIGINT)
		{
			// Si readline a été interrompu par Ctrl-C, il peut retourner une ligne vide
			// On efface alors cette ligne et redessine proprement le prompt
			write(1, "\n", 1);
			rl_on_new_line();
			rl_replace_line("", 0);
			rl_redisplay();
			g_signal = 0;
			free(input); // On libère même si input est ""
			continue;	 // On recommence à zéro
		}
		else if (g_signal == SIGQUIT)
		{
			// Rien à faire (Ctrl-\ doit être silencieux)
			g_signal = 0;
		};
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
		// cleanup_shell(shell);
	}
}
