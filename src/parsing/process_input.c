/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 17:57:59 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/01 12:55:32 by hugoganet        ###   ########.fr       */
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
	// Ajoute le pointer des tokens à la structure du shell pour free proprement
	shell->tokens = tokens;
	// print_token_list(tokens);
	ast_root = build_ast(tokens);
	if (!ast_root)
	{
		ft_putendl_fd("minishell: error: failed to build AST", 2);
		free_token_list(tokens);
		return;
	}
	expand_vars(ast_root, shell);
	pretty_print_ast(ast_root, 0, "ROOT");
	// print_ast_cmd_node(ast_root->args);
	// Ajoute le pointer de l'AST à la structure du shell pour free proprement
	shell->ast = ast_root;
	shell->last_exit_status = execute_ast(ast_root, shell->env_list, shell);
}