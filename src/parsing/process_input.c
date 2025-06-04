/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 17:57:59 by hugoganet         #+#    #+#             */
/*   Updated: 2025/06/04 11:37:17 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



void print_ast(t_ast *node, int depth)
{
    if (!node)
        return;

    // Affichage de l'indentation en fonction de la profondeur
    for (int i = 0; i < depth; i++)
        printf("  ");

    // Affichage du type et de la chaîne associée (si elle existe)
    printf("Type: %d", node->type);
    if (node->str)
        printf(", Str: '%s'", node->str);
    printf("\n");

    // Appel récursif pour les enfants
    if (node->left)
    {
        for (int i = 0; i < depth + 1; i++)
            printf("  ");
        printf("Left:\n");
        print_ast(node->left, depth + 2);
    }

    if (node->right)
    {
        for (int i = 0; i < depth + 1; i++)
            printf("  ");
        printf("Right:\n");
        print_ast(node->right, depth + 2);
    }
}

/**
 * @brief Traite la ligne de commande : tokenisation, parsing logique à venir.
 *
 * @param input La ligne brute saisie par l'utilisateur
 * @param shell Structure principale du shell
 */
 void process_input(char *input, t_shell *shell)
{
	t_token *tokens;
	t_ast	*ast_root;

	(void)shell; // Utile plus tard
	// Tokenisation de la ligne d'entrée
	tokens = tokenize(input);
	if (!tokens)
	{
		// Si la tokenisation échoue, on affiche un message d'erreur et on quitte
		ft_putendl_fd("minishell: error: failed to tokenize input", 2);
		return;
	}
	ast_root = build_ast(tokens);

	printf("Avant expansion :\n");
	print_ast(ast_root, 0);

	// expand_vars(ast_root, shell);

	// printf("Après expansion :\n");
    // print_ast(ast);
	
	// Libération de la liste de tokens
	free_token_list(tokens);
}




 /* void process_input(char *input, t_shell *shell)
{
	t_token *tokens;

	(void)shell; // Utile plus tard
	// Tokenisation de la ligne d'entrée
	tokens = tokenize(input);
	if (!tokens)
	{
		// Si la tokenisation échoue, on affiche un message d'erreur et on quitte
		ft_putendl_fd("minishell: error: failed to tokenize input", 2);
		return;
	}
	// Affichage de la liste de tokens pour debug
	//print_token_list(tokens);
	// TODO : parser_logique(tokens);
	// Libération de la liste de tokens
	free_token_list(tokens);
} */
