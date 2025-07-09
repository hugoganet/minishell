/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialisation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 13:28:30 by elaudrez          #+#    #+#             */
/*   Updated: 2025/07/09 18:26:02 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Initialise les variables de base du shell
 * @param shell Pointeur vers la structure à initialiser
 */
static void	init_shell_vars(t_shell *shell)
{
	shell->env = NULL;
	shell->env_list = NULL;
	shell->export_list = NULL;
	shell->tokens = NULL;
	shell->ast = NULL;
	shell->last_exit_status = 0;
	shell->heredoc_fds = NULL;
}

/**
 * @brief Initialise la structure du shell : copie de l'environnement,
 *        initialisation des signaux et du statut de sortie.
 *
 * @param shell Pointeur vers la structure à initialiser.
 * @param envp Environnement système (non modifiable directement).
 */
void	init_shell(t_shell *shell, char **envp)
{
	init_shell_vars(shell);
	shell->env_list = init_env_list(envp);
	shell->export_list = init_env_list(envp);
	sort_list(&shell->export_list);
	shell->env = env_to_char_array(shell->env_list);
	if (!shell->env_list || !shell->env || !shell->export_list)
	{
		ft_putendl_fd
			("minishell: error: failed to initialize environment list", 2);
		cleanup_shell(shell);
		exit(1);
	}	
	configure_shlvl(envp, shell->env_list);
}
