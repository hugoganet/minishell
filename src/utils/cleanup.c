/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 16:25:16 by hugoganet         #+#    #+#             */
/*   Updated: 2025/06/11 14:32:59 by hugoganet        ###   ########.fr       */
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
	if (shell->env_list)
	{
		printf("Freeing environment list...\n");
		free_env_list(shell->env_list);
	}
	if (shell->env)
	{
		printf("Freeing environment array...\n");	
		free_env(shell->env);
	}
	if (shell->tokens)
	{
		printf("Freeing token list...\n");
		free_token_list(shell->tokens);
	}
	if (shell->ast)
		free_ast(shell->ast);
}
