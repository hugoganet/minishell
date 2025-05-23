/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 17:26:05 by hugoganet         #+#    #+#             */
/*   Updated: 2025/05/23 13:07:22 by hugoganet        ###   ########.fr       */
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
	shell.env = copy_env(envp);
	shell.last_exit_status = 0;
	init_signals();
	while (1)
	{
		input = prompt_readline();
		if (!input)
		{
			write(1, "exit\n", 5);
			break;
		}
		free(input);
	}
	rl_clear_history();
	free_env(shell.env);
	return (0);
}