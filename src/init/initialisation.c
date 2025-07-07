/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialisation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 13:28:30 by elaudrez          #+#    #+#             */
/*   Updated: 2025/07/07 21:26:11 by hugoganet        ###   ########.fr       */
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
 * @param env_list Liste chaînée d'environnement.
 */
void	init_shell(t_shell *shell, char **envp, t_env *env_list)
{
	init_shell_vars(shell);
	shell->env = copy_env(envp);
	if (!shell->env)
	{
		ft_putendl_fd("minishell: error: failed to copy environment", 2);
		exit(1);
	}
	shell->export_list = init_env_list(envp);
	sort_list(&shell->export_list);
	env_list = init_env_list(envp);
	if (!env_list)
	{
		ft_putendl_fd(
			"minishell: error: failed to initialize environment list", 2);
		free_env(shell->env);
		exit(1);
	}
	shell->env_list = env_list;
	configure_shlvl(envp, env_list);
	if (shell->env)
	{
		free_env(shell->env);
		shell->env = env_to_char_array(env_list);
	}
}

/**
 * @brief Copie l'environnement système dans un tableau de chaînes alloué.
 *
 * @param envp L'environnement original (reçu dans main).
 * @return char** Une copie modifiable de l'environnement.
 */
char	**copy_env(char **envp)
{
	int		i;
	char	**env;

	i = 0;
	while (envp[i])
		i++;
	env = ft_calloc((i + 1), sizeof(char *));
	if (!env)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		env[i] = strdup(envp[i]);
		if (!env[i])
		{
			free_env(env);
			return (NULL);
		}
		i++;
	}
	return (env);
}
