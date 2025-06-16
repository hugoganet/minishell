/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 17:21:39 by hugoganet         #+#    #+#             */
/*   Updated: 2025/06/16 15:11:29 by hugoganet        ###   ########.fr       */
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
        printf("\n[print_ast_cmd_node] argv is NULL\n");
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

/**
 * @brief Affiche joliment l'arbre de syntaxe avec indentation et couleurs.
 *
 * Si le nœud est de type CMD, affiche aussi le tableau `args`.
 *
 * @param node Pointeur vers le nœud courant
 * @param depth Niveau d'indentation
 */
void pretty_print_ast(t_ast *node, int depth)
{
	int i;
	int j;

	if (!node)
		return;
	i = 0;
	while (i++ < depth)
		printf("    ");
	printf("└── %s%s%s", token_color(node->type),
		   token_type_str(node->type), COLOR_RESET);
	if (node->str)
		printf(" %s", node->str);
	printf("\n");
	// Affichage des arguments si disponibles
	if (node->args)
	{
		j = 0;
		while (node->args[j])
		{
			i = 0;
			while (i++ <= depth)
				printf("    ");
			printf("arg[%d]: %s\n", j, node->args[j]);
			j++;
		}
	}
	pretty_print_ast(node->left, depth + 1);
	pretty_print_ast(node->right, depth + 1);
}
