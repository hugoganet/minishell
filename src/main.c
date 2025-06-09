/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 17:26:05 by hugoganet         #+#    #+#             */
/*   Updated: 2025/06/04 18:52:33 by hugoganet        ###   ########.fr       */
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
	t_env *env_list; // Liste chaînée d'environnement

	(void)argc;
	(void)argv;
	// Initialise le shell avec l'environnement
	init_shell(&shell, envp);
	// Initialisation de la liste chaînée d'environnement
	env_list = init_env_list(envp);
	if (!env_list)
	{
		ft_putendl_fd("minishell: error: failed to initialize environment list", 2);
		free_env(shell.env);
		exit(1);
	}
	// TODO : initialiser shell.env_list ailleurs
	shell.env_list = env_list;
	// print_env_list(env_list);
	shell_loop(&shell); // Boucle principale du shell
	
	free_env(shell.env); // Libère la mémoire allouée pour l'environnement
	return (0);
}

 
/* int main(int argc, char **argv, char **envp)
{
	t_shell shell; // Structure du shell

	(void)argc;
	(void)argv;
	init_shell(&shell, envp); // Initialise le shell avec l'environnement
	shell_loop(&shell); // Boucle principale du shell
	rl_clear_history(); // Efface l'historique
	free_env(shell.env); // Libère la mémoire allouée pour l'environnement
	return (0);
} */
