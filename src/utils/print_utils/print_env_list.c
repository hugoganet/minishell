/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_env_list.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 14:46:52 by hugoganet         #+#    #+#             */
/*   Updated: 2025/06/03 14:55:10 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Affiche la liste chaînée d'environnement.
 *
 * Cette fonction parcourt la liste chaînée d'environnement et affiche
 * chaque variable d'environnement au format "key=value".
 *
 * @param env La tête de la liste chaînée d'environnement.
 */
void print_env_list(t_env *env)
{
	t_env *current;

	current = env;
	if (!current)
	{
		printf("No environment variables set.\n");
		return;
	}
	printf("\n=== Environment Variables ===\n");
	while (current)
	{
		if (current->value)
			printf("%s=%s\n", current->key, current->value);
		else
			printf("%s=\n", current->key);
		current = current->next;
	}
	printf("=============================\n\n");
}