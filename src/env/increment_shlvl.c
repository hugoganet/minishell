/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   increment_shlvl.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 17:32:00 by elaudrez          #+#    #+#             */
/*   Updated: 2025/07/08 17:41:34 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Incrémente la variable SHLVL dans l'environnement.
 *
 * Cette fonction recherche la variable d'environnement SHLVL, la convertit
 * en entier, l'incrémente de 1, et met à jour sa valeur dans l'environnement.
 * Si SHLVL n'existe pas, elle est créée avec la valeur "1".
 * Si SHLVL n'est pas un nombre valide ou est négative, elle est réinitialisée
 * à "1".
 * Si SHLVL dépasse 999, elle est réinitialisée à "1" (comportement de Bash).
 *
 * @param env_list La liste chaînée des variables d'environnement
 * @return int 1 en cas de succès, 0 en cas d'erreur
 */

static int	if_no_shlvl(t_env **shlvl_node, t_env **env_list)
{
	*shlvl_node = create_env_pair("SHLVL", "1");
	if (!*shlvl_node)
		return (0);
	(*shlvl_node)->next = (*env_list)->next;
	(*env_list)->next = *shlvl_node;
	return (1);
}

int	increment_shlvl(t_env *env_list)
{
	t_env	*shlvl_node;
	int		level;
	char	*new_value;

	if (!env_list)
		return (1);
	// shlvl prend le head de la liste d'environnement
	shlvl_node = env_list;
	// On parcourt la liste pour trouver SHLVL
	while (shlvl_node && ft_strcmp(shlvl_node->key, "SHLVL") != 0)
		shlvl_node = shlvl_node->next;
	// Si SHLVL n'existe pas, on le crée, si shlvl_node est NULL et que la fonction if_no_shlvl retourne 0, on quitte
	if (!shlvl_node && !if_no_shlvl(&shlvl_node, &env_list))
		return (1);
	// On convertit la valeur de SHLVL en entier
	level = ft_atoi(shlvl_node->value);
	// Si la conversion échoue ou si le niveau est négatif, on le réinitialise à 1
	if (level < 0)
		level = 1;
	// Sinon on l'incrémente de 1, et si le niveau dépasse 999, on le remet à 1
	else if (++level > 999)
		level = 1;
	// On convertit le niveau en chaîne de caractères et on met à jour la valeur de SHLVL
	new_value = ft_itoa(level);
	if (!new_value)
		return (1);
	free(shlvl_node->value);
	shlvl_node->value = new_value;
	return (0);
}
