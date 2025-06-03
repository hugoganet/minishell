/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 14:44:49 by hugoganet         #+#    #+#             */
/*   Updated: 2025/06/03 15:13:34 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Initialise une liste chaînée d'environnement à partir de envp.
 *
 * Cette fonction parcourt le tableau envp et crée pour chaque variable
 * une nouvelle cellule de la liste chaînée t_env, séparant la clé et la valeur.
 *
 * @param envp Tableau de chaînes représentant l'environnement système.
 * @return t_env* Pointeur vers la tête de la liste chaînée d'environnement.
 */
t_env *init_env_list(char **envp)
{
	int i;
	t_env *head;
	t_env *current;
	char *equal;

	head = NULL;
	current = NULL;
	i = 0;
	// Parcourt le tableau envp et crée des nœuds pour chaque variable
	while (envp && envp[i])
	{
		t_env *node = malloc(sizeof(t_env));
		if (!node)
			return NULL;
		equal = ft_strchr(envp[i], '=');
		if (equal)
		{
			node->key = ft_substr(envp[i], 0, equal - envp[i]);
			node->value = ft_strdup(equal + 1);
		}
		else
		{
			node->key = ft_strdup(envp[i]);
			node->value = NULL;
		}
		node->next = NULL;
		if (!head)
			head = node;
		else
			current->next = node;
		current = node;
		i++;
	}
	return head;
}