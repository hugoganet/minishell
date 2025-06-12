/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_executor.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 15:57:23 by hugoganet         #+#    #+#             */
/*   Updated: 2025/06/11 16:22:13 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Exécute récursivement un arbre de syntaxe AST.
 *
 * @param node Le nœud courant de l’AST à exécuter
 * @param env  La liste des variables d’environnement
 * @return Code de retour de la commande (à transmettre à `$?`)
 */
int execute_ast(t_ast *node, t_env *env_list, t_shell *shell)
{
	if (!node)
		return (1);
	if (node->type == PIPE)
		return (execute_pipe_node(node, env_list, shell));
	if (node->type == CMD)
		return (exec_cmd(node, env_list));
	// Les autres types (PIPE, REDIR, etc.) seront gérés plus tard
	ft_putendl_fd("execute_ast: unsupported node type (WIP)\n", STDERR_FILENO);
	return (1);
}