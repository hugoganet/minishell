/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_resolution.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 17:40:32 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/04 09:06:22 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"

/**
 * @brief Concatène trois chaînes de caractères en une seule.
 *
 * Alloue de la mémoire pour une nouvelle chaîne contenant
 * la concaténation de s1, s2 et s3.
 *
 * @param s1 Première chaîne à concaténer
 * @param s2 Deuxième chaîne à concaténer
 * @param s3 Troisième chaîne à concaténer
 * @return Nouvelle chaîne concaténée, ou NULL en cas d'erreur
 */
static char *ft_strjoin_three(char *s1, char *s2, char *s3)
{
	char *tmp;
	char *res;

	tmp = ft_strjoin(s1, s2);
	if (!tmp)
		return (NULL);
	res = ft_strjoin(tmp, s3);
	free(tmp);
	return (res);
}

/**
 * @brief Vérifie si une commande avec chemin est accessible.
 *
 * Teste l'accessibilité en exécution d'une commande qui contient
 * déjà un chemin (absolu ou relatif).
 *
 * @param cmd_name Nom de la commande avec chemin
 * @return Copie du nom de commande si accessible, NULL sinon
 */
static char *check_direct_path(char *cmd_name)
{
	if (ft_strchr(cmd_name, '/'))
	{
		if (access(cmd_name, X_OK) == 0)
			return (ft_strdup(cmd_name));
		return (NULL);
	}
	return (NULL);
}

/**
 * @brief Cherche une commande dans un répertoire spécifique du PATH.
 *
 * Construit le chemin complet en joignant le répertoire avec le nom
 * de la commande, puis teste son accessibilité.
 *
 * @param path_dir Répertoire du PATH à tester
 * @param cmd_name Nom de la commande
 * @return Chemin complet si trouvé, NULL sinon
 */
static char *search_in_path_dir(char *path_dir, char *cmd_name)
{
	char *full_path;

	full_path = ft_strjoin_three(path_dir, "/", cmd_name);
	if (!full_path)
		return (NULL);
	if (access(full_path, X_OK) == 0)
		return (full_path);
	free(full_path);
	return (NULL);
}

/**
 * @brief Cherche une commande dans tous les répertoires du PATH.
 *
 * Parcourt chaque répertoire du PATH et teste l'accessibilité
 * de la commande dans chacun d'eux.
 *
 * @param paths Tableau des répertoires du PATH
 * @param cmd_name Nom de la commande à chercher
 * @return Chemin complet si trouvé, NULL sinon
 */
static char *search_in_path_dirs(char **paths, char *cmd_name)
{
	char *result;
	int i;

	i = 0;
	while (paths[i])
	{
		result = search_in_path_dir(paths[i], cmd_name);
		if (result)
		{
			free_split(paths);
			return (result);
		}
		i++;
	}
	free_split(paths);
	return (NULL);
}

/**
 * @brief Résout le chemin absolu d'une commande en fonction du PATH.
 *
 * Si la commande contient un '/', elle est testée telle quelle.
 * Sinon, on parcourt chaque dossier du PATH pour tester son accessibilité.
 *
 * @param cmd_name Le nom de la commande (ex : "ls")
 * @param env Liste chaînée des variables d'environnement
 * @return Un chemin absolu vers la commande, ou NULL si introuvable
 */
char *resolve_command_path(char *cmd_name, t_env *env)
{
	char **paths;
	char *path_var;
	char *direct_result;

	direct_result = check_direct_path(cmd_name);
	if (direct_result)
		return (direct_result);
	if (ft_strchr(cmd_name, '/'))
		return (NULL);
	path_var = get_env_value(env, "PATH");
	if (!path_var)
		return (NULL);
	paths = ft_split(path_var, ':');
	if (!paths)
		return (NULL);
	return (search_in_path_dirs(paths, cmd_name));
}
