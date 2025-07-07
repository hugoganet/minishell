/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 17:47:04 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/07 13:57:04 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Recherche la valeur d'une variable d’environnement dans la liste.
 *
 * Cette fonction parcourt la liste chaînée `t_env` et retourne la valeur
 * associée à la clé recherchée (par exemple "PATH", "HOME", etc.).
 *
 * @param env Liste chaînée représentant les variables d’environnement
 * @param key Nom de la variable à rechercher
 * @return `char *` Valeur de la variable, ou NULL si introuvable.
 */
char	*get_env_value(t_env *env_list, const char *key)
{
	while (env_list)
	{
		if (ft_strcmp(env_list->key, key) == 0)
			return (env_list->value);
		env_list = env_list->next;
	}
	return (NULL);
}

/**
 * @brief Compte le nombre de variables d’environnement dans la liste.
 */
static int	count_env_vars(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		count++;
		env = env->next;
	}
	return (count);
}

/**
 * @brief Concatène les paires clé=valeur dans un tableau de chaînes.
 * Cette fonction remplit un tableau de chaînes de caractères
 * avec les paires `KEY=VALUE` à partir de la liste chaînée d’environnement.
 * 
 * @param env Liste chaînée d’environnement
 * @param array Tableau de chaînes de caractères à remplir
 * @return `int` 0 en cas de succès, 1 en cas d’erreur d’allocation mémoire.
 */
static int	join_env_vars(t_env *env, char **array)
{
	char	*joined;
	int		i;

	i = 0;
	while (env)
	{
		joined = ft_strjoin(env->key, "=");
		if (!joined)
			return (1);
		array[i] = ft_strjoin(joined, env->value);
		if (!array[i])
		{
			free(joined);
			return (1);
		}
		free(joined);
		i++;
		env = env->next;
	}
	return (0);
}

/**
 * @brief Convertit la liste chaînée d’environnement en tableau de chaînes.
 *
 * Cette fonction est utilisée pour exécuter une commande avec `execve`,
 * qui attend un tableau `char **envp` de la forme :
 *   envp[0] = "PATH=/usr/bin"
 *   envp[1] = "HOME=/home/user"
 *   ...
 *
 * @param env Liste chaînée d’environnement
 * @return `char **` Tableau NULL-terminé contenant les paires `KEY=VALUE`
 */
char	**env_to_char_array(t_env *env)
{
	int		count;
	char	**array;

	count = count_env_vars(env);
	array = ft_calloc(count + 1, sizeof(char *));
	if (!array)
		return (NULL);
	if (join_env_vars(env, array) != 0)
	{
		free(array);
		return (NULL);
	}
	return (array);
}
