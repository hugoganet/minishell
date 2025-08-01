/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_executor.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 15:57:23 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/09 17:55:03 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ast.h"
#include "pipe.h"

/**
 * @brief Exécute récursivement un arbre de syntaxe AST.
 *
 * @param node Le nœud courant de l'AST à exécuter
 * @param env  La liste des variables d'environnement
 * @return Code de retour de la commande (à transmettre à `$?`)
 */
int	execute_ast(t_ast *node, t_env *env_list, t_shell *shell)
{
	t_ast	*cmd_node;

	if (!node)
		return (1);
	if (node->type == PIPE)
		return (execute_pipe_node(node, env_list, shell));
	else if (node->type == REDIR_INPUT || node->type == REDIR_OUTPUT
		|| node->type == REDIR_APPEND || node->type == HEREDOC)
	{
		cmd_node = find_cmd_node(node);
		if (!cmd_node)
			return (apply_parent_redirections(node, shell));
		return (exec_cmd(cmd_node, env_list, node, shell));
	}
	else if (node->type == CMD)
		return (exec_cmd(node, env_list, node, shell));
	return (1);
}
