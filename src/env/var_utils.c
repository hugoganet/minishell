/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 17:32:00 by elaudrez          #+#    #+#             */
/*   Updated: 2025/07/03 16:04:51 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


/**
 * @brief Incrémente la variable SHLVL dans l'environnement.
 *
 * Cette fonction recherche la variable d'environnement SHLVL, la convertit
 * en entier, l'incrémente de 1, et met à jour sa valeur dans l'environnement.
 * Si SHLVL n'existe pas, elle est créée avec la valeur "1".
 * Si SHLVL n'est pas un nombre valide ou est négative, elle est réinitialisée à "1".
 * Si SHLVL dépasse 999, elle est réinitialisée à "1" (comportement de Bash).
 *
 * @param env_list La liste chaînée des variables d'environnement
 * @return int 0 en cas de succès, 1 en cas d'erreur
 */
int increment_shlvl(t_env *env_list)
{
	t_env *shlvl_node;
	int level;
	char *new_value;

	// Rechercher le nœud SHLVL dans la liste d'environnement
	shlvl_node = env_list;
	while (shlvl_node)
	{
		if (ft_strcmp(shlvl_node->key, "SHLVL") == 0)
			break;
		shlvl_node = shlvl_node->next;
	}
	// Si SHLVL n'existe pas, créer un nouveau nœud avec la valeur "1"
	if (!shlvl_node)
	{
		shlvl_node = create_env_pair("SHLVL", "1");
		if (!shlvl_node)
			return (1);
		// Ajouter le nouveau nœud à la liste
		shlvl_node->next = env_list->next;
		env_list->next = shlvl_node;
		return (0);
	}
	// Convertir la valeur actuelle en entier
	level = ft_atoi(shlvl_node->value);
	// Si la valeur n'est pas numérique ou est négative, réinitialiser à 1
	if (level < 0)
		level = 0;
	// Incrémenter la valeur
	level++;
	// Si la valeur dépasse 999, la réinitialiser à 1 (comportement de Bash)
	if (level > 999)
		level = 1;
	// Convertir la nouvelle valeur en chaîne de caractères
	new_value = ft_itoa(level);
	if (!new_value)
		return (1);
	// Libérer l'ancienne valeur et mettre à jour
	free(shlvl_node->value);
	shlvl_node->value = new_value;
	return (0);
}
