/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialisation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 13:28:30 by elaudrez          #+#    #+#             */
/*   Updated: 2025/06/03 17:45:12 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Initialise la structure du shell : copie de l'environnement,
 *        initialisation des signaux et du statut de sortie.
 *
 * @param shell Pointeur vers la structure à initialiser.
 * @param envp Environnement système (non modifiable directement).
 */
void init_shell(t_shell *shell, char **envp)
{
	// Copie de l'environnement dans la structure shell
	shell->env = copy_env(envp);
	if (!shell->env)
	{
		ft_putendl_fd("minishell: error: failed to copy environment", 2);
		exit(1);
	}
	// Initialisation du statut de sortie
	shell->last_exit_status = 0;
	// Initialisation des signaux
	init_signals();
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
		i++;
	}
	return (env);
}

/**
 * @brief Libère la mémoire allouée pour l'environnement.
 *
 * @param env Le tableau de chaînes alloué à libérer.
 */
void free_env(char **env)
{
	int i = 0;

	// Libère chaque chaîne dans le tableau
	while (env && env[i])
		free(env[i++]);
	// Libère le tableau lui-même
	free(env);
}

/**
 * @brief Libère la mémoire de la structure t_env
 * 
 * @param env La liste chaînée d'environnement à libérer.
 */
void free_env_list(t_env *env)
{
	t_env *temp;

	if (!env)
		return;
	// Libère chaque nœud de la liste chaînée
	while (env)
	{
		temp = env;
		env = env->next;
		free(temp->key);
		free(temp->value);
		free(temp);
	}
}
