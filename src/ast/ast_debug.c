/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_debug.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 11:50:57 by hugoganet         #+#    #+#             */
/*   Updated: 2025/05/28 11:51:00 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ast.h"

/**
 * @brief Affiche une indentation visuelle pour représenter l'arbre.
 *
 * @param depth Niveau de profondeur dans l'arbre
 */
static void print_indent(int depth)
{
	int i = 0;

	while (i < depth)
	{
		printf("  ");
		i++;
	}
}

/**
 * @brief Affiche un nœud de commande (AST_CMD).
 *
 * @param cmd Tableau de chaînes représentant la commande
 * @param depth Profondeur actuelle dans l’arbre (indentation)
 */
static void print_cmd_node(char **cmd, int depth)
{
	int i = 0;

	print_indent(depth);
	printf("[CMD]");
	while (cmd && cmd[i])
	{
		printf(" %s", cmd[i]);
		i++;
	}
	printf("\n");
}

/**
 * @brief Affiche récursivement l’arbre d’exécution de façon lisible.
 *
 * @param node Nœud AST à afficher
 * @param depth Niveau d’indentation
 */
void print_ast(t_ast *node, int depth)
{
	if (!node)
		return;

	if (node->type == AST_CMD)
	{
		print_cmd_node(node->cmd, depth);
		return;
	}

	print_indent(depth);
	if (node->type == AST_PIPE)
		printf("[PIPE]\n");
	else if (node->type == AST_AND)
		printf("[AND]\n");
	else if (node->type == AST_OR)
		printf("[OR]\n");
	else if (node->type == AST_GROUP)
		printf("[GROUP]\n");

	print_ast(node->left, depth + 1);
	print_ast(node->right, depth + 1);
}