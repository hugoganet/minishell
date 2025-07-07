/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 17:26:05 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/07 15:27:28 by elaudrez         ###   ########.fr       */
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
char *prompt_readline_tester(void)
{
	char *input;

	// Utiliser get_next_line si le terminal n'est pas interactif
	// c'est-à-dire si l'entrée standard n'est pas un terminal. (pour le testeur)
	if (!isatty(STDIN_FILENO))
	{
		// Mode non interactif : lecture depuis stdin avec get_next_line
		input = get_next_line(STDIN_FILENO);
		return (input);
	}
	// Affiche le prompt
	input = readline("");
	// Si le signal est SIGINT (Ctrl+C)
	if (g_signal == SIGINT)
	{
		// on libère l'entrée
		free(input);
		// input = NULL;
		// Réinitialise le signal
		g_signal = 0;
		// ligne vide pour éviter d'interrompre la boucle
		return (ft_strdup(""));
	}
	// Si l'entrée n'est pas NULL et n'est pas vide
	if (input && *input)
		// Ajoute l'entrée à l'historique
		add_history(input);
	return (input);
}

int main(int argc, char **argv, char **envp)
{
	t_shell shell;
	t_env *env_list;
	char *input;

	(void)argc;
	(void)argv;
	env_list = NULL;
	input = NULL;
	init_signals();
	init_shell(&shell, envp, env_list);
	if (!shell.env || !shell.env_list)
	{
		ft_putendl_fd("minishell: error: failed to initialize shell environment", 2);
		cleanup_shell(&shell);
		exit(1);
	}
	if (argc > 1)
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
		cleanup_shell(&shell);
		return (shell.last_exit_status);
	}
	else
		shell_loop(&shell);
	cleanup_shell(&shell);
	return (0);
}
