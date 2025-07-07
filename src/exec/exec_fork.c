/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_fork.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 00:00:00 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/07 00:00:00 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"
#include <errno.h>

static void	run_child_process_no_heredoc(char **argv, t_env *env,
		t_ast *ast, t_shell *shell)
{
	char	*path;
	char	**envp;

	reset_signals_in_child();
	if (setup_redirections(ast) != 0)
	{
		cleanup_shell(shell);
		exit(1);
	}
	path = resolve_command_path(argv[0], env);
	envp = env_to_char_array(env);
	if (!path || !envp)
	{
		free_child_exec(path, envp, shell);
		exit(127);
	}
	if (execve(path, argv, envp) == -1)
	{
		if (errno != ENOEXEC)
			perror("minishell: execve");
		free_child_exec(path, envp, shell);
		exit(127);
	}
}

static int	handle_child_status(int status)
{
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			write(STDOUT_FILENO, "\n", 1);
		else if (WTERMSIG(status) == SIGQUIT)
			write(STDOUT_FILENO, "Quit (core dumped)\n", 20);
		return (128 + WTERMSIG(status));
	}
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

int	execute_fork_process(t_ast *cmd_node, t_env *env,
		t_ast *ast_root, t_shell *shell)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
	{
		perror("minishell: fork");
		return (1);
	}
	if (pid == 0)
		run_child_process(cmd_node->args, env, ast_root, shell);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	waitpid(pid, &status, 0);
	init_signals();
	close_all_heredoc_fds(shell);
	free_all_heredoc_fds(shell);
	return (handle_child_status(status));
}

int	execute_fork_process_no_heredoc(t_ast *cmd_node, t_env *env,
		t_ast *ast_root, t_shell *shell)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
	{
		perror("minishell: fork");
		return (1);
	}
	if (pid == 0)
		run_child_process_no_heredoc(cmd_node->args, env, ast_root, shell);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	waitpid(pid, &status, 0);
	init_signals();
	return (handle_child_status(status));
}
