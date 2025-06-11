/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 14:44:49 by hugoganet         #+#    #+#             */
/*   Updated: 2025/06/10 13:32:41 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Initialise un nœud de la liste d'environnement.
 *
 * Cette fonction sépare la clé et la valeur d'une variable
 * d'environnement et les stocke dans le nœud.
 *
 * @param node Pointeur vers le nœud à remplir (déjà alloué).
 * @param env_var Chaîne de type "KEY=VALUE".
 * @return int 0 en cas de succès, 1 en cas d’erreur d’allocation.
 */
int init_env_list_node(t_env *node, char *env_var)
{
	char *equal;

	if (!node || !env_var)
		return (1);

	equal = ft_strchr(env_var, '=');
	if (equal)
	{
		node->key = ft_substr(env_var, 0, equal - env_var);
		node->value = ft_strdup(equal + 1);
	}
	else
	{
		node->key = ft_strdup(env_var);
		node->value = NULL;
	}
	// Vérifie les erreurs d’allocation
	if (!node->key || (equal && !node->value))
		return (1);
	return (0);
}

/**
 * @brief Crée un nœud t_env à partir d'une chaîne "KEY=VALUE".
 *
 * @param env_var Variable d’environnement au format "KEY=VALUE"
 * @return t_env* Nœud initialisé ou NULL en cas d’erreur
 */
static t_env *create_env_node(char *env_var)
{
	t_env *node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->next = NULL;
	if (init_env_list_node(node, env_var) != 0)
	{
		free(node->key);
		free(node->value);
		free(node);
		return (NULL);
	}
	return (node);
}

/**
 * @brief Initialise une liste chaînée d’environnement à partir de envp.
 *
 * En cas d’erreur d’allocation, libère toute la mémoire déjà allouée.
 *
 * @param envp Tableau de chaînes d’environnement.
 * @return t_env* Pointeur vers la tête de la liste ou NULL en cas d’échec.
 */
t_env *init_env_list(char **envp)
{
	int i = 0;
	t_env *head = NULL;
	t_env *current = NULL;
	t_env *node;

	while (envp && envp[i])
	{
		node = create_env_node(envp[i]);
		if (!node)
		{
			free_env_list(head);
			return (NULL);
		}
		if (!head)
			head = node;
		else
			current->next = node;
		current = node;
		i++;
	}
	return (head);
}
