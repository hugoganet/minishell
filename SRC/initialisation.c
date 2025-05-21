/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialisation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 13:28:30 by elaudrez          #+#    #+#             */
/*   Updated: 2025/05/21 16:28:48 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Crée une copie de l'environnement système.
 *
 * Cette fonction parcourt le tableau `env` et crée une nouvelle copie
 * de toutes les chaînes de caractères, allouée dynamiquement.
 *
 * @param env Un tableau de chaînes représentant l'environnement (terminé par NULL).
 * @return Un nouveau tableau de chaînes dupliquées, ou NULL en cas d'erreur d'allocation.
 */
char	**get_env(char **env)
{
	char	**new_env;
	int		i;

	i = 0;
	while (env[i])
		i++;
	new_env = malloc((i + 1) * sizeof(char *));
	if (!new_env)
		return (NULL);
	i = 0;
	while(env[i])
	{
		new_env[i] = ft_strdup(env[i]);
		i++;
	}
	new_env[i] = NULL;
	return (new_env);
}

/**
 * @brief Extrait et découpe la variable d’environnement PATH.
 *
 * Cette fonction cherche la ligne contenant "PATH=" dans l’environnement donné,
 * puis la découpe selon le séparateur `:` pour récupérer chaque répertoire.
 *
 * @param env Un tableau de chaînes représentant l’environnement (terminé par NULL).
 * @return Un tableau de chemins (terminé par NULL), ou NULL si PATH est absent ou en cas d'erreur.
 */
char	**split_path(char **env)
{
	int		i;
	char	**path;

	i = 0;
	if (!env)
		return (NULL);
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
			break ;
		i++;
	}
	if (env[i] == NULL)
		return (NULL);
	path = ft_split(env[i] + 5, ':');
	if (!path)
		return (NULL);
	return (path);
}

/**
 * @brief Construit le chemin absolu d'une commande à partir de l’environnement.
 *
 * Utilise les chemins définis dans la variable PATH pour construire un chemin complet
 * vers la commande `cmd`. Vérifie si la commande est accessible (`X_OK`).
 *
 * @param env L’environnement système (tableau de chaînes, terminé par NULL).
 * @param cmd Le nom de la commande à rechercher (ex: "ls").
 * @return Une chaîne représentant le chemin complet vers la commande,
 *         ou NULL si la commande n’a pas été trouvée ou en cas d’erreur.
 *
 * @note Le tableau retourné est alloué dynamiquement. L’appelant doit libérer la mémoire.
 */
char	*get_path(char **env, char *cmd)
{
	char	**path;
	char	*pathname;
	char	*tmp;
	int		i;

	path = split_path(env);
	if (!path)
		return (NULL);
	i = 0;
	pathname = NULL;
	while (path[i])
	{
		tmp = ft_strjoin(path[i], "/");
		pathname = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(pathname, X_OK) == 0)
		{
			free_tab(path);
			return (pathname);
		}
		free(pathname);
		i++;
	}
	free_tab(path);
	return (NULL);
}

int	main(int ac, char *av[], char **env)
{
	(void)ac;
	(void)av;
	int	i;
	char **new_env;
	char	*input;

	i = 0;
	new_env = get_env(env);
	
	while (1)
	{
		input = readline("Minishell > ");
		if (!input)
			break;
		if(input)
			add_history(input);
		printf("%s\n", input);
		free(input);
	}
	return (0);
}



