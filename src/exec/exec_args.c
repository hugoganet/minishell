/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_args.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 08:32:17 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/09 18:30:37 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"

/**
 * @brief Valide la commande dans un nœud de l'AST.
 * Cette fonction vérifie si la commande n'est pas NULL,
 * si elle a au moins un argument, et si le premier argument n'est pas vide.
 */
int	validate_command(t_ast *cmd_node)
{
	if (!cmd_node || !cmd_node->args)
		return (0);
	if (!cmd_node->args[0])
		return (0);
	if (cmd_node->args[0][0] == '\0')
		return (0);
	return (1);
}

/**
 * @brief Filtre les arguments vides d'un tableau d'arguments.
 * Cette fonction parcourt le tableau d'arguments et libère la mémoire
 * des chaînes vides, tout en décalant les arguments non vides vers la gauche.
 * Le tableau est terminé par un pointeur NULL.
 */
void	filter_empty_args(char **args)
{
	int	read_idx;
	int	write_idx;

	if (!args)
		return ;
	read_idx = 0;
	write_idx = 0;
	while (args[read_idx])
	{
		if (args[read_idx][0] == '\0')
		{
			free(args[read_idx]);
		}
		else
		{
			if (write_idx != read_idx)
				args[write_idx] = args[read_idx];
			write_idx++;
		}
		read_idx++;
	}
	args[write_idx] = NULL;
}
