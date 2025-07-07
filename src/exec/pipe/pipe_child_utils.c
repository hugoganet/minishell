#include "pipe.h"

void setup_child_pipes(int **pipes, int cmd_count, int child_index, t_shell *shell);
void setup_child_stdin(int **pipes, int child_index, t_shell *shell);
void setup_child_stdout(int **pipes, int child_index, int cmd_count, t_shell *shell);
void close_all_child_pipes(int **pipes, int cmd_count);
void execute_pipeline_child(t_ast **commands, int **pipes, pid_t *pids, int cmd_count, int index, t_env *env, t_shell *shell);

void setup_child_pipes(int **pipes, int cmd_count, int child_index, t_shell *shell)
{
	setup_child_stdin(pipes, child_index, shell);
	setup_child_stdout(pipes, child_index, cmd_count, shell);
	close_all_child_pipes(pipes, cmd_count);
}

void setup_child_stdin(int **pipes, int child_index, t_shell *shell)
{
	if (child_index > 0)
	{
		if (dup2(pipes[child_index - 1][0], STDIN_FILENO) == -1)
		{
			perror("minishell: dup2 stdin");
			cleanup_shell(shell);
			exit(1);
		}
	}
}

void setup_child_stdout(int **pipes, int child_index, int cmd_count, t_shell *shell)
{
	if (child_index < cmd_count - 1)
	{
		if (dup2(pipes[child_index][1], STDOUT_FILENO) == -1)
		{
			perror("minishell: dup2 stdout");
			cleanup_shell(shell);
			exit(1);
		}
	}
}

void close_all_child_pipes(int **pipes, int cmd_count)
{
	int j;

	j = 0;
	while (j < cmd_count - 1)
	{
		close(pipes[j][0]);
		close(pipes[j][1]);
		j++;
	}
}

void execute_pipeline_child(t_ast **commands, int **pipes, pid_t *pids,
							int cmd_count, int index, t_env *env, t_shell *shell)
{
	t_ast *current_cmd;
	int status;

	reset_signals_in_child();
	setup_heredoc_redirection(shell);
	current_cmd = commands[index];
	setup_child_pipes(pipes, cmd_count, index, shell);
	close_all_heredoc_fds(shell);
	cleanup_child_memory_early(commands, pipes, pids, cmd_count - 1);
	status = exec_cmd_no_heredoc(current_cmd, env, current_cmd, shell);
	cleanup_shell(shell);
	exit(status);
}
