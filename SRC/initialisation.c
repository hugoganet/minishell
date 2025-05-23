/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialisation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 13:28:30 by elaudrez          #+#    #+#             */
/*   Updated: 2025/05/23 12:10:30 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	while (envp[i])	// Compte le nombre d'éléments dans envp
		i++;
	env = ft_calloc((i + 1),  sizeof(char *)); // Alloue de la mémoire pour le tableau et set à NULL
	if (!env)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		env[i] = strdup(envp[i]); // Copie chaque élément de envp dans le nouveau tableau
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

	while (env && env[i])
		free(env[i++]);
	free(env);
}
