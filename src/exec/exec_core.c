/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_core.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 08:32:17 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/04 13:22:24 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"

/**
 * @brief Exécute une commande simple (non composée) en forkant un enfant.
 *
 * - Récupère le nœud CMD
 * - Traite les heredocs
 * - Valide la commande
 * - Fork, exécute, puis attend la fin du processus enfant
 *
 * @param cmd_node Le nœud CMD (ou un nœud supérieur contenant le CMD)
 * @param env Liste des variables d'environnement
 * @param ast_root Racine de l'AST courant
 * @param shell Structure du shell principal
 * @return Code de retour de la commande exécutée
 */
int exec_cmd(t_ast *cmd_node, t_env *env, t_ast *ast_root, t_shell *shell)
{
	int heredoc_status;
	int validation_result;

	cmd_node = find_cmd_node(ast_root);
	heredoc_status = process_heredocs(ast_root, shell);
	if (heredoc_status == 130)
		return (130);
	validation_result = validate_command(cmd_node);
	if (validation_result == -1)
		return (0); // Commande vide : retourner 0 comme Bash
	if (!validation_result)
		return (127);
	if (is_builtin(cmd_node))
		return (builtin_exec(cmd_node, shell));
	return (execute_fork_process(cmd_node, env, ast_root, shell));
}
