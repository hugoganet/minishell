/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 17:26:05 by hugoganet         #+#    #+#             */
/*   Updated: 2025/05/23 13:17:44 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Point d’entrée du programme Minishell.
 *
 * @param argc Nombre d’arguments (non utilisé)
 * @param argv Liste des arguments (non utilisé)
 * @param envp Environnement système
 * @return int Code de retour du shell
 */
int main(int argc, char **argv, char **envp)
{
	t_shell shell; // Structure du shell

	(void)argc;
	(void)argv;
	init_shell(&shell, envp); // Initialise le shell avec l'environnement
	shell_loop(&shell); // Boucle principale du shell
	rl_clear_history(); // Efface l'historique
	free_env(shell.env); // Libère la mémoire allouée pour l'environnement
	return (0);
}
