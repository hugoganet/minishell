/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 16:25:16 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/07 21:57:01 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Libère toutes les ressources allouées par le shell.
 *
 * Cette fonction est appelée avant de quitter le programme,
 * que ce soit après un `exit`, un `Ctrl+D`, ou une erreur critique.
 *
 * @param shell Pointeur vers la structure principale du shell.
 */
static void	cleanup_shell_data(t_shell *shell)
{
	if (shell->env_list)
	{
		free_env_list(shell->env_list);
		shell->env_list = NULL;
	}
	if (shell->env)
	{
		free_env(shell->env);
		shell->env = NULL;
	}
	if (shell->tokens)
	{
		free_token_list(shell->tokens);
		shell->tokens = NULL;
	}
	if (shell->ast)
	{
		free_ast(shell->ast);
		shell->ast = NULL;
	}
	if (shell->export_list)
	{
		free_env_list(shell->export_list);
		shell->export_list = NULL;
	}
}

void	cleanup_shell(t_shell *shell)
{
	if (!shell)
		return ;
	cleanup_shell_data(shell);
	close_all_heredoc_fds(shell);
	free_all_heredoc_fds(shell);
}
