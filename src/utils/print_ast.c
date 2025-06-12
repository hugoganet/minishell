/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 17:21:39 by hugoganet         #+#    #+#             */
/*   Updated: 2025/06/11 15:59:58 by hugoganet        ###   ########.fr       */
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

/**
 * @brief Affiche l’AST sous forme indentée avec couleurs.
 *
 * Chaque ligne de sortie suit le format :
 *   └── [TYPE] "valeur"   (avec couleur selon le type)
 *
 * @param node Le noeud courant de l’AST à afficher
 * @param depth La profondeur dans l’arbre (0 à la racine)
 */
void pretty_print_ast(t_ast *node, int depth)
{
    int i;

    // Cas de base : noeud vide
    if (!node)
        return;
    // Indentation : 4 espaces par niveau
    i = 0;
    while (i < depth)
    {
        printf("    ");
        i++;
    }
    // Affichage du noeud avec couleur selon le type
    printf("└── %s%s%s", token_color(node->type),
           token_type_str(node->type), COLOR_RESET);
    // Affiche la chaîne associée si elle existe
    if (node->str)
        printf(" \"%s\"", node->str);
    printf("\n");
    // Appelle récursif pour les sous-arbres
    pretty_print_ast(node->left, depth + 1);
    pretty_print_ast(node->right, depth + 1);
}
