/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_core.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 08:32:17 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/06 20:47:44 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"

static int	exec_builtin_with_redirections(t_ast *cmd_node, t_ast *ast_root,
		t_shell *shell)
{
	int	saved_stdin;
	int	saved_stdout;
	int	heredoc_status;
	int	setup_result;
	int	builtin_result;

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

int	exec_cmd(t_ast *cmd_node, t_env *env, t_ast *ast_root, t_shell *shell)
{
	int	heredoc_status;
	int	validation_result;

	cmd_node = find_cmd_node(ast_root);
	heredoc_status = process_heredocs(ast_root, shell);
	if (heredoc_status == 130)
		return (130);
	if (cmd_node && cmd_node->args)
		filter_empty_args(cmd_node->args);
	validation_result = validate_command(cmd_node);
	if (validation_result == -1)
	{
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

int	exec_cmd_no_heredoc(t_ast *cmd_node, t_env *env, t_ast *ast_root,
		t_shell *shell)
{
	int	validation_result;

	cmd_node = find_cmd_node(ast_root);
	if (cmd_node && cmd_node->args)
		filter_empty_args(cmd_node->args);
	validation_result = validate_command(cmd_node);
	if (validation_result == -1)
	{
		print_command_not_found_error("");
		return (127);
	}
	if (!validation_result)
		return (127);
	if (is_builtin(cmd_node))
	{
		if (is_redirection(ast_root->type) == true)
			return (exec_builtin_with_redirections_no_heredoc(cmd_node,
					ast_root, shell));
		return (builtin_exec(cmd_node, shell));
	}
	return (execute_fork_process_no_heredoc(cmd_node, env, ast_root, shell));
}
