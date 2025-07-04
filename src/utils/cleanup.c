/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 16:25:16 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/04 19:35:46 by hugoganet        ###   ########.fr       */
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
void cleanup_shell(t_shell *shell)
{
	if (!shell)
		return;
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
