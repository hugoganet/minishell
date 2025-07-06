/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_core.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 08:32:17 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/06 16:11:52 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"

/**
 * @brief Exécute un builtin avec des redirections appliquées correctement.
 *
 * Sauvegarde les descripteurs originaux, applique les redirections,
 * exécute le builtin, puis restaure les descripteurs originaux.
 *
 * @param cmd_node Le nœud contenant la commande builtin
 * @param ast_root Le nœud racine contenant les redirections
 * @param shell Structure du shell principal
 * @return Code de retour du builtin exécuté
 */
static int exec_builtin_with_redirections(t_ast *cmd_node, t_ast *ast_root,
										  t_shell *shell)
{
	int saved_stdin;
	int saved_stdout;
	int heredoc_status;
	int setup_result;
	int builtin_result;

	if (save_std_descriptors(&saved_stdin, &saved_stdout) != 0)
		return (1);
	heredoc_status = process_heredocs(ast_root, shell);
	if (heredoc_status == 130)
	{
		restore_std_descriptors(saved_stdin, saved_stdout);
		return (130);
	}
	setup_result = setup_redirections(ast_root);
	if (setup_result != 0)
	{
		restore_std_descriptors(saved_stdin, saved_stdout);
		return (1);
	}
	builtin_result = builtin_exec(cmd_node, shell);
	restore_std_descriptors(saved_stdin, saved_stdout);
	return (builtin_result);
}

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
	{
		// Commande vide : afficher l'erreur et retourner 127 comme Bash
		print_command_not_found_error("");
		return (127);
	}
	if (!validation_result)
		return (127);
	if (is_builtin(cmd_node))
	{
		if (is_redirection(ast_root->type) == true)
			return (exec_builtin_with_redirections(cmd_node, ast_root, shell));
		return (builtin_exec(cmd_node, shell));
	}
	return (execute_fork_process(cmd_node, env, ast_root, shell));
}

/**
 * @brief Exécute une commande sans traiter les heredocs (pour les processus enfants de pipe).
 *
 * Version spéciale d'exec_cmd utilisée dans les processus enfants des pipes,
 * où les heredocs ont déjà été traités par le processus parent.
 *
 * @param cmd_node Le nœud CMD (ou un nœud supérieur contenant le CMD)
 * @param env Liste des variables d'environnement
 * @param ast_root Racine de l'AST courant
 * @param shell Structure du shell principal
 * @return Code de retour de la commande exécutée
 */
int exec_cmd_no_heredoc(t_ast *cmd_node, t_env *env, t_ast *ast_root, t_shell *shell)
{
	int validation_result;

	cmd_node = find_cmd_node(ast_root);
	validation_result = validate_command(cmd_node);
	if (validation_result == -1)
	{
		// Commande vide : afficher l'erreur et retourner 127 comme Bash
		print_command_not_found_error("");
		return (127);
	}
	if (!validation_result)
		return (127);
	if (is_builtin(cmd_node))
	{
		if (is_redirection(ast_root->type) == true)
			return (exec_builtin_with_redirections_no_heredoc(cmd_node, ast_root, shell));
		return (builtin_exec(cmd_node, shell));
	}
	return (execute_fork_process_no_heredoc(cmd_node, env, ast_root, shell));
}
