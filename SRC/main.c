/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 17:26:05 by hugoganet         #+#    #+#             */
/*   Updated: 2025/05/23 13:14:02 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Initialise le shell, affiche le prompt, et lance la boucle interactive.
 *
 * @param argc Nombre d’arguments (non utilisé)
 * @param argv Liste des arguments (non utilisé)
 * @param envp Environnement système
 * @return int Code de retour du shell
 */
int main(int argc, char **argv, char **envp)
{
	t_shell shell;
	char *input;

	(void)argc;
	(void)argv;
	init_shell(&shell, envp); // Initialise le shell avec l'environnement
	while (1)
	{
		input = prompt_readline(); // Affiche le prompt et lit l'entrée utilisateur
		if (!input) // Si Ctrl+D est pressé, on sort de la boucle
		{
			write(1, "exit\n", 5);
			break;
		}
		free(input); // Libère la mémoire allouée pour l'entrée
	}
	rl_clear_history(); // Efface l'historique
	free_env(shell.env); // Libère la mémoire allouée pour l'environnement
	return (0);
}
