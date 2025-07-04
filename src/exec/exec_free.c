/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_free.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 08:32:17 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/04 09:06:22 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"

/**
 * @brief Libère les ressources allouées dans le processus enfant.
 *
 * @param path Chemin de la commande à libérer
 * @param envp Tableau d'environnement à libérer
 * @param shell Structure du shell à nettoyer
 */
void free_child_exec(char *path, char **envp, t_shell *shell)
{
	free(path);
	free_env(envp);
	cleanup_shell(shell);
}

/**
 * @brief Libère un tableau de chaînes de caractères.
 *
 * Parcourt le tableau et libère chaque chaîne individuellement,
 * puis libère le tableau lui-même.
 *
 * @param arr Tableau de chaînes à libérer
 */
void free_string_array(char **arr)
{
	int i;

	if (!arr)
		return;
	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}
