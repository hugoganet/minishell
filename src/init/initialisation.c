/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialisation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 13:28:30 by elaudrez          #+#    #+#             */
/*   Updated: 2025/05/28 15:56:45 by hugoganet        ###   ########.fr       */
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
	// ? Where is the memory allocation ?!
	shell->env = copy_env(envp); // Copie de l'environnement
	shell->last_exit_status = 0; // Initialisation du statut de sortie
	init_signals(); // Initialisation des signaux
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
		free(env[i++]); // Libère chaque chaîne
	free(env); // Libère le tableau
}
