/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_args.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 08:32:17 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/06 20:47:47 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"

/**
 * @brief Valide la commande avant l'exécution.
 *
 * @param cmd_node Nœud de commande à valider
 * @return 1 si valide, 0 sinon, -1 si commande vide
 */
int validate_command(t_ast *cmd_node)
{
	if (!cmd_node || !cmd_node->args)
		return (0);
	if (!cmd_node->args[0])
		return (0);
	// Si la commande est marquée pour suppression, c'est comme s'il n'y avait pas de commande
	if (cmd_node->args[0][0] == '\x01' && cmd_node->args[0][1] == '\0')
		return (0);
	// Seule la commande elle-même (args[0]) vide est considérée comme "commande vide"
	if (cmd_node->args[0][0] == '\0')
		return (-1);
	return (1);
}

/**
 * @brief Filtre les arguments vides qui résultent d'expansions de variables échouées.
 *        Les arguments marqués avec \x01 sont supprimés du tableau.
 *
 * @param args Tableau d'arguments à filtrer
 */
void filter_empty_args(char **args)
{
	int read_idx;
	int write_idx;

	if (!args)
		return;

	read_idx = 0;
	write_idx = 0;

	while (args[read_idx])
	{
		// Si l'argument est marqué pour suppression (\x01), on le libère et l'ignore
		if (args[read_idx][0] == '\x01' && args[read_idx][1] == '\0')
		{
			free(args[read_idx]);
		}
		else
		{
			// Sinon, on le garde en le décalant si nécessaire
			if (write_idx != read_idx)
				args[write_idx] = args[read_idx];
			write_idx++;
		}
		read_idx++;
	}

	// Marquer la fin du tableau filtré
	args[write_idx] = NULL;
}
