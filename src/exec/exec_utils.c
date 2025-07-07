/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 08:25:59 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/06 16:32:23 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"

void	reset_signals_in_child(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

int	is_directory(char *path)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) == 0)
	{
		if (S_ISDIR(path_stat.st_mode))
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(path, STDERR_FILENO);
			ft_putendl_fd(": is a directory", STDERR_FILENO);
			return (1);
		}
	}
	return (0);
}

int	save_std_descriptors(int *saved_stdin, int *saved_stdout)
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

void	restore_std_descriptors(int saved_stdin, int saved_stdout)
{
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
}

int	exec_builtin_with_redirections_no_heredoc(t_ast *cmd_node, t_ast *ast_root,
		t_shell *shell)
{
	int	saved_stdin;
	int	saved_stdout;
	int	result;

	if (save_std_descriptors(&saved_stdin, &saved_stdout) != 0)
		return (1);
	if (setup_redirections(ast_root) != 0)
	{
		restore_std_descriptors(saved_stdin, saved_stdout);
		return (1);
	}
	result = builtin_exec(cmd_node, shell);
	restore_std_descriptors(saved_stdin, saved_stdout);
	return (result);
}
