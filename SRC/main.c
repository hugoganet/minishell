/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 17:26:05 by hugoganet         #+#    #+#             */
/*   Updated: 2025/05/22 17:52:01 by hugoganet        ###   ########.fr       */
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
	t_shell shell;
	char *input;

	(void)argc;
	(void)argv;
	shell.env = copy_env(envp);
	shell.last_exit_status = 0;
	init_signals();
	while (1)
	{
		input = readline("minishell> ");
		if (!input) // Ctrl-D
		{
			write(1, "exit\n", 5);
			break;
		}
		if (*input)
			add_history(input);
		free(input); // On libère l’entrée, parsing à faire dans les prochaines phases
	}
	free_env(shell.env);
	return (0);
}