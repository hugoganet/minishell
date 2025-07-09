/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 17:26:05 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/08 17:04:23 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;
	char	*input;

	(void)argc;
	(void)argv;
	input = NULL;
	init_signals();
	init_shell(&shell, envp);
	if (!shell.env || !shell.env_list)
	{
		ft_putendl_fd(
			"minishell: error: failed to initialize shell environment", 2);
		cleanup_shell(&shell);
		exit(1);
	}
	if (argc > 1)
		return (process_main_argv_as_input(input, &shell, argv));
	else
		shell_loop(&shell);
	cleanup_shell(&shell);
	return (0);
}
