/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_core.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 08:32:17 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/06 10:41:37 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"

/**
 * @brief Sauvegarde les descripteurs de fichier stdin et stdout.
 *
 * @param saved_stdin Pointeur pour stocker le descripteur stdin sauvegardé
 * @param saved_stdout Pointeur pour stocker le descripteur stdout sauvegardé
 * @return 0 en cas de succès, 1 en cas d'erreur
 */
static int save_std_descriptors(int *saved_stdin, int *saved_stdout)
{
	*saved_stdin = dup(STDIN_FILENO);
	*saved_stdout = dup(STDOUT_FILENO);
	if (*saved_stdin == -1 || *saved_stdout == -1)
	{
		perror("dup");
		return (1);
	}
	return (0);
}

/**
 * @brief Restaure les descripteurs de fichier originaux et les ferme.
 *
 * @param saved_stdin Descripteur stdin sauvegardé
 * @param saved_stdout Descripteur stdout sauvegardé
 */
static void restore_std_descriptors(int saved_stdin, int saved_stdout)
{
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
}

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
		return (0); // Commande vide : retourner 0 comme Bash
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