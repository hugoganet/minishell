/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_ast.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 21:05:00 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/09 18:26:35 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	expand_single_argument(char **arg, t_shell *shell)
{
	char	*expanded_arg;
	bool	expanded_to_empty;

	expanded_arg = expand_variables_with_flag(*arg, shell->env_list,
			shell->last_exit_status, &expanded_to_empty);
	if (expanded_arg)
	{
		if (expanded_to_empty && ft_strlen(expanded_arg) == 0)
		{
			free(expanded_arg);
			free(*arg);
			*arg = ft_strdup("\x01");
		}
		else
		{
			free(*arg);
			*arg = expanded_arg;
		}
	}
}

/**
 * @brief Traverse l'AST et expanse les variables dans les arguments des cmd.
 *
 * @param node Le nœud racine de l'AST à traiter.
 * @param shell Structure principale du shell.
 */
void	expand_ast_arguments(t_ast *node, t_shell *shell)
{
	int	i;

	if (!node)
		return ;
	expand_ast_arguments(node->left, shell);
	expand_ast_arguments(node->right, shell);
	if (node->type == CMD && node->args)
	{
		i = 0;
		while (node->args[i])
		{
			expand_single_argument(&node->args[i], shell);
			i++;
		}
	}
}
