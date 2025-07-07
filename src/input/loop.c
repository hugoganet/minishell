/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 13:16:41 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/07 18:05:07 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Boucle principale du shell : lit les entrées, les traite,
 *        et quitte proprement sur Ctrl+D.
 *
 * @param shell Pointeur vers la structure du shell.
 */
void	shell_loop(t_shell *shell)
{
	char	*input;

	while (1)
	{
		input = prompt_readline();
		if (!input)
		{
			write(1, "exit\n", 5);
			cleanup_shell(shell);
			exit(0);
		}
		if (g_signal == SIGINT)
		{
			shell->last_exit_status = 130;
			g_signal = 0;
		}
		if (!is_line_empty(input))
		{
			if (!is_syntax_valid(input, shell))
				process_input(input, shell);
		}
		free_loop(input, shell);
	}
}

/**
 * @brief Traite l'argument passé en ligne de commande comme une entrée.
 * 
 * @param input Pointeur vers la chaîne d'entrée.
 * @param shell Pointeur vers la structure du shell.
 * @param argv Tableau des arguments de la ligne de commande.
 * @return Le statut de sortie du shell après traitement de l'entrée.
 */
int	process_main_argv_as_input(char *input, t_shell *shell, char **argv)
{
	input = ft_strdup(argv[1]);
	if (input && !is_line_empty(input))
	{
		if (!is_syntax_valid(input, shell))
			process_input(input, shell);
	}
	free(input);
	free_ast(shell->ast);
	shell->ast = NULL;
	free_token_list(shell->tokens);
	shell->tokens = NULL;
	cleanup_shell(shell);
	return (shell->last_exit_status);
}
