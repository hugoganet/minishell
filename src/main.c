/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 17:26:05 by hugoganet         #+#    #+#             */
/*   Updated: 2025/06/30 15:08:14 by hugoganet        ###   ########.fr       */
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
	t_shell	shell; // Structure du shell
	t_env	*env_list; // Liste chaînée d'environnement
	char	*input;

	(void)argc;
	(void)argv;
	env_list = NULL;
	input = NULL;
	// Initialise les signaux pour le shell
	init_signals();
	// Initialise le shell avec l'environnement
	init_shell(&shell, envp, env_list);
	if (!shell.env || !shell.env_list)
	{
		ft_putendl_fd("minishell: error: failed to initialize shell environment", 2);
		cleanup_shell(&shell);
		exit(1);
	}
	if (argc > 1) // Utilisé le tester uniquement
	{
		input = ft_strdup(argv[1]);
		if (input && !is_line_empty(input))
		{
			if (!is_syntax_valid(input, &shell))
				process_input(input, &shell);
		}
		free(input);
		free_ast(shell.ast);
		shell.ast = NULL;
		free_token_list(shell.tokens);
		shell.tokens = NULL;
	}
	else
		shell_loop(&shell);

	cleanup_shell(&shell);
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
