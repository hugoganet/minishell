/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_args.c                                        :+:      :+:    :+:   */
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
 * @brief Filtre les arguments vides d'un tableau de chaînes, en préservant argv[0].
 *
 * Cette fonction modifie le tableau sur place pour enlever les chaînes
 * vides dans les arguments (pas la commande elle-même).
 * Elle libère la mémoire des chaînes vides pour éviter les fuites.
 *
 * @param args Le tableau d'arguments à filtrer.
 */
void filter_empty_args(char **args)
{
	int i;
	int j;

	if (!args || !args[0])
		return;
	// On commence à 1 pour préserver argv[0] (le nom de la commande)
	i = 1;
	j = 1;
	while (args[i])
	{
		if (args[i][0] == '\0')
		{
			free(args[i]);
		}
		else
		{
			if (i != j)
				args[j] = args[i];
			j++;
		}
		i++;
	}
	args[j] = NULL;
}

/**
 * @brief Valide la commande avant l'exécution.
 *
 * @param cmd_node Nœud de commande à valider
 * @return 1 si valide, 0 sinon
 */
int validate_command(t_ast *cmd_node)
{
	if (!cmd_node || !cmd_node->args)
		return (0);
	filter_empty_args(cmd_node->args);
	if (!cmd_node->args[0])
		return (0);
	if (cmd_node->args[0][0] == '\0')
	{
		ft_putendl_fd(": command not found", STDERR_FILENO);
		return (0);
	}
	return (1);
}
