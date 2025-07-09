/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 17:57:59 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/09 14:22:35 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "expansion.h"

/**
 * @brief Traite la ligne de commande : tokenisation, parsing logique à venir.
 *
 * @note Appeler `pretty_print_ast(ast_root, 0, "ROOT");` pour afficher l'AST
 *
 * @param input La ligne brute saisie par l'utilisateur
 * @param shell Structure principale du shell
 */
void	process_input(char *input, t_shell *shell)
{
	t_token	*tokens;
	t_ast	*ast_root;

	// Créé la liste chaînée de tokens à partir de l'entrée utilisateur
	tokens = tokenize(input);
	if (!tokens)
		return ;
	shell->tokens = tokens;
	// Crée la liste chaînée de l'AST.
	ast_root = build_ast(tokens);
	if (!ast_root)
	{
		ft_putendl_fd("minishell: error: failed to build AST", 2);
		free_token_list(tokens);
		return ;
	}
	// On parcours tous les ARGs de l'AST pour vérifier si il y'a des variables à expanser.
	expand_ast_arguments(ast_root, shell);
	// On set l'AST dans la structure du shell.
	shell->ast = ast_root;
	shell->last_exit_status = execute_ast(ast_root, shell->env_list, shell);
}
