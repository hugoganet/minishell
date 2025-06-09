/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resolve_command_path.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 17:40:32 by hugoganet         #+#    #+#             */
/*   Updated: 2025/06/04 18:56:41 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Concatène trois chaînes de caractères en une seule.
 *
 * @return `char *` La chaine concaténée
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
 * @brief Résout le chemin absolu d'une commande en fonction du PATH.
 *
 * Si la commande contient un '/', elle est testée telle quelle.
 * Sinon, on parcourt chaque dossier du PATH pour tester son accessibilité.
 *
 * @param cmd_name Le nom de la commande (ex : "ls")
 * @param env Liste chaînée des variables d’environnement
 * @return Un chemin absolu vers la commande, ou NULL si introuvable
 */
char *resolve_command_path(char *cmd_name, t_env *env)
{
	char **paths;
	char *path_var;
	char *full_path;
	int i;

	// Cas d'une commande avec chemin absolu ou relatif déjà fourni
	if (ft_strchr(cmd_name, '/'))
	{
		if (access(cmd_name, X_OK) == 0)
			return (ft_strdup(cmd_name));
		return (NULL);
	}
	
	// Récupère la valeur de PATH
	path_var = get_env_value(env, "PATH");
	// dprintf(2, "resolve_command_path: PATH=%s\n", path_var);
	if (!path_var)
		return (NULL);
	paths = ft_split(path_var, ':');
	if (!paths)
		return (NULL);

	// Teste chaque dossier du PATH
	i = 0;
	while (paths[i])
	{
		full_path = ft_strjoin_three(paths[i], "/", cmd_name);
		if (!full_path)
			break;
		if (access(full_path, X_OK) == 0)
			return (free_split(paths), full_path);
		free(full_path);
		i++;
	}
	free_split(paths);
	return (NULL);
}