/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 17:26:05 by hugoganet         #+#    #+#             */
/*   Updated: 2025/05/23 12:57:21 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Point d'entrée du shell. Initialise l'environnement, les signaux,
 *        puis boucle sur l'affichage du prompt.
 *
 * @param argc Nombre d’arguments (non utilisé)
 * @param argv Liste des arguments (non utilisé)
 * @param envp Environnement système
 * @return int Code de retour du shell
 */
int main(int argc, char **argv, char **envp)
{
	t_shell	shell;  // Structure pour stocker l'environnement et le statut de sortie
	char	*input; // Variable pour stocker l'entrée utilisateur

	(void)argc;
	(void)argv;
	shell.env = copy_env(envp); // Copie de l'environnement système
	shell.last_exit_status = 0; // Initialisation du statut de sortie
	init_signals(); // Initialisation des signaux
	while (1)
	{
		input = readline("minishell> "); // Affichage du prompt et lecture de l'entrée utilisateur
		if (!input) // Si l'entrée est NULL (Ctrl+D)
		{
			write(1, "exit\n", 5);
			break;
		}
		if (*input) // Si l'entrée n'est pas vide
			add_history(input); // Ajout de l'entrée à l'historique
		free(input); // On libère l’entrée
	}
	rl_clear_history(); // Libération de l'historique
	free_env(shell.env); // Libération de l'environnement
	return (0);
}