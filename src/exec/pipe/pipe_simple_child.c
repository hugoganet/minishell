#include "pipe.h"

void setup_left_child_process(int fd[2], t_ast *node, t_env *env, t_shell *shell);
void setup_right_child_process(int fd[2], t_ast *node, t_env *env, t_shell *shell);
int wait_for_children(pid_t left_pid, pid_t right_pid);
int handle_pipe_execution_error(int fd[2], pid_t left_pid);
// close_pipe_fds si elle existe

void setup_left_child_process(int fd[2], t_ast *node, t_env *env, t_shell *shell)
{
	int status;

	reset_signals_in_child();
	setup_heredoc_redirection(shell);
	if (dup2(fd[1], STDOUT_FILENO) == -1)
	{
		perror("minishell: dup2");
		cleanup_shell(shell);
		exit(1);
	}
	close(fd[0]);
	close(fd[1]);
	close_all_heredoc_fds(shell);
	status = exec_cmd_no_heredoc(node->left, env, node->left, shell);
	cleanup_shell(shell);
	exit(status);
}

void setup_right_child_process(int fd[2], t_ast *node, t_env *env, t_shell *shell)
{
	int status;

	reset_signals_in_child();
	if (dup2(fd[0], STDIN_FILENO) == -1)
	{
		perror("minishell: dup2");
		cleanup_shell(shell);
		exit(1);
	}
	close(fd[0]);
	close(fd[1]);
	close_all_heredoc_fds(shell);
	status = exec_cmd_no_heredoc(node->right, env, node->right, shell);
	cleanup_shell(shell);
	exit(status);
}

int wait_for_children(pid_t left_pid, pid_t right_pid)
{
	int status;

	waitpid(left_pid, &status, 0);
	waitpid(right_pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			write(STDOUT_FILENO, "\n", 1);
		else if (WTERMSIG(status) == SIGQUIT)
			write(STDOUT_FILENO, "Quit (core dumped)\n", 20);
		return (128 + WTERMSIG(status));
	}
	return (1);
}

int handle_pipe_execution_error(int fd[2], pid_t left_pid)
{
	close_pipe_fds(fd);
	if (left_pid > 0)
	{
		kill(left_pid, SIGTERM);
		waitpid(left_pid, NULL, 0);
	}
	init_signals();
	return (1);
}
