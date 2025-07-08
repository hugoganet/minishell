/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 17:57:59 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/08 18:25:41 by hugoganet        ###   ########.fr       */
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

	tokens = tokenize(input);
	if (!tokens)
		return ;
	shell->tokens = tokens;
	ast_root = build_ast(tokens);
	if (!ast_root)
	{
		ft_putendl_fd("minishell: error: failed to build AST", 2);
		free_token_list(tokens);
		return ;
	}
	expand_ast_arguments(ast_root, shell);
	shell->ast = ast_root;
	shell->last_exit_status = execute_ast(ast_root, shell->env_list, shell);
}
