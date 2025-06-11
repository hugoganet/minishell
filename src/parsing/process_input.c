/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 17:57:59 by hugoganet         #+#    #+#             */
/*   Updated: 2025/06/11 13:52:35 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

	// Tokenisation de la ligne d'entrée
	tokens = tokenize(input);
	if (!tokens)
	{
		// Si la tokenisation échoue, on affiche un message d'erreur et on quitte
		ft_putendl_fd("minishell: error: failed to tokenize input", 2);
		return;
	}
	// printf("Tokens:\n");
	// for (t_token *tmp = tokens; tmp; tmp = tmp->next)
	// printf("  Token Type: %d, Str: '%s'\n", tmp->type, tmp->str);
	// print_token_list(tokens);
	// TODO : ajouter le pointer de l'AST à la structure shell pour libérer tout dans cleanup_shell
	print_token_list(tokens);
	ast_root = build_ast(tokens);
	if (!ast_root)
	{
		ft_putendl_fd("minishell: error: failed to build AST", 2);
		free_token_list(tokens);
		return;
	}
	// pretty_print_ast(ast_root, 0);
	execute_ast(ast_root, shell->env_list);
	// Libération de la liste de tokens
	free_token_list(tokens);
	// Libération de l'AST
	free_ast(ast_root);
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
