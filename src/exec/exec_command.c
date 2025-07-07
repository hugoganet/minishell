/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 08:32:17 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/06 12:29:37 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"
#include <errno.h>

t_ast	*find_cmd_node(t_ast *node)
{
	t_ast	*found;

	if (!node)
		return (NULL);
	if (node->type == CMD)
		return (node);
	found = find_cmd_node(node->left);
	if (found)
		return (found);
	return (find_cmd_node(node->right));
}

static char	*resolve_and_check_command(char **argv, t_env *env, t_shell *shell)
{
	char	*path;

	path = resolve_command_path(argv[0], env);
	if (!path)
	{
		print_command_not_found_error(argv[0]);
		cleanup_shell(shell);
		exit(127);
	}
	if (is_directory(path))
	{
		free(path);
		cleanup_shell(shell);
		exit(126);
	}
	return (path);
}

void	run_child_process(char **argv, t_env *env,
		t_ast *ast, t_shell *shell)
{
	char	*path;
	char	**envp;

	reset_signals_in_child();
	setup_heredoc_redirection(shell);
	if (setup_redirections(ast) != 0)
	{
		cleanup_shell(shell);
		exit(1);
	}
	path = resolve_and_check_command(argv, env, shell);
	envp = env_to_char_array(env);
	if (!envp)
	{
		perror("minishell: env malloc");
		cleanup_shell(shell);
		exit(1);
	}
	if (execve(path, argv, envp) == -1)
	{
		if (errno != ENOEXEC)
			perror("minishell: execve");
		free_child_exec(path, envp, shell);
		exit(127);
	}
}
