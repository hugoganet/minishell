/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 17:21:39 by hugoganet         #+#    #+#             */
/*   Updated: 2025/06/03 17:33:30 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Affiche un tableau de chaînes `argv` (terminé par NULL).
 *
 * Cette fonction est principalement utilisée pour le debug, afin de vérifier
 * que le tableau d’arguments généré depuis l’AST est correct avant exécution.
 *
 * @param argv Le tableau d’arguments à afficher (type `char **`)
 */
void print_ast_cmd_node(char **argv)
{
	int i;

	if (!argv)
	{
		printf("[print_ast_cmd_node] argv is NULL\n");
		return;
	}
	printf("\n=== [AST → argv] Command arguments: ===\n");
	i = 0;
	while (argv[i])
	{
		printf("  argv[%d] = \"%s\"\n", i, argv[i]);
		i++;
	}
	printf("=============================\n\n");
}
