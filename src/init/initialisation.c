/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialisation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 13:28:30 by elaudrez          #+#    #+#             */
/*   Updated: 2025/06/25 11:00:19 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Initialise la structure du shell : copie de l'environnement,
 *        initialisation des signaux et du statut de sortie.
 *
 * @param shell Pointeur vers la structure à initialiser.
 * @param envp Environnement système (non modifiable directement).
 * @param env_list Liste chaînée d'environnement (initialisée dans init_env_list).
 */
void init_shell(t_shell *shell, char **envp, t_env *env_list)
{
	// Copie de l'environnement dans la structure shell
	shell->env = copy_env(envp);
	if (!shell->env)
	{
		ft_putendl_fd("minishell: error: failed to copy environment", 2);
		exit(1);
	}
	// Initialisation de la liste chaînée d'environnement
	env_list = init_env_list(envp);
	if (!env_list)
	{
		ft_putendl_fd("minishell: error: failed to initialize environment list", 2);
		free_env(shell->env);
		exit(1);
	}
	shell->env_list = env_list;

	// Incrémente SHLVL pour chaque instance de sous-shell
	if (increment_shlvl(env_list) != 0)
	{
		ft_putendl_fd("minishell: warning: failed to increment SHLVL", 2);
		// On continue malgré l'erreur - ce n'est pas critique
	}

	// Maintenant, nous devons synchroniser env avec env_list car SHLVL a été modifié
	if (shell->env)
	{
		free_env(shell->env);					  // Libère l'ancienne copie
		shell->env = env_to_char_array(env_list); // Crée une nouvelle copie à jour
	}

	// Initialisation du statut de sortie
	shell->last_exit_status = 0;
}

/**
 * @brief Copie l'environnement système dans un tableau de chaînes alloué.
 *
 * @param envp L'environnement original (reçu dans main).
 * @return char** Une copie modifiable de l'environnement.
 */
char **copy_env(char **envp)
{
	int i;
	char **env;

	i = 0;
	// Compte le nombre d'éléments dans envp
	while (envp[i])
		i++;
	// Alloue de la mémoire pour le tableau et set à NULL
	env = ft_calloc((i + 1), sizeof(char *));
	if (!env)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		// Copie chaque élément de envp dans le nouveau tableau
		env[i] = strdup(envp[i]);
		if (!env[i])
		{
			free_env(env); // Libère la mémoire en cas d'erreur
			return (NULL);
		}
		i++;
	}
	return (env);
}
