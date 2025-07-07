#include "pipe.h"

int execute_complex_pipeline(t_ast *node, t_env *env, t_shell *shell);
int setup_pipeline_execution(t_ast *node, t_ast ***commands, int ***pipes, pid_t **pids, int *pipes_created, t_shell *shell);
int create_pipeline_processes(t_ast **commands, int **pipes, pid_t *pids, int cmd_count, t_env *env, t_shell *shell);
int wait_for_all_processes(pid_t *pids, int cmd_count);
void close_parent_pipes(int **pipes, int cmd_count);

int execute_complex_pipeline(t_ast *node, t_env *env, t_shell *shell)
{
	t_ast **commands;
	int **pipes;
	pid_t *pids;
	int pipes_created;
	int final_status;
	int setup_result;

	commands = NULL;
	pipes = NULL;
	pids = NULL;
	setup_result = setup_pipeline_execution(node, &commands, &pipes, &pids,
											&pipes_created, shell);
	if (setup_result != 0)
		return (setup_result);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	if (create_pipeline_processes(commands, pipes, pids,
								  count_pipeline_commands(node), env, shell) != 0)
	{
		cleanup_pipeline_resources(commands, pipes, pids, pipes_created);
		init_signals();
		return (1);
	}
	close_parent_pipes(pipes, count_pipeline_commands(node));
	final_status = wait_for_all_processes(pids, count_pipeline_commands(node));
	init_signals();
	close_all_heredoc_fds(shell);
	free_all_heredoc_fds(shell);
	cleanup_pipeline_memory_only(commands, pipes, pids, pipes_created);
	return (final_status);
}

int setup_pipeline_execution(t_ast *node, t_ast ***commands, int ***pipes,
							 pid_t **pids, int *pipes_created, t_shell *shell)
{
	int cmd_count;
	int index;

	cmd_count = count_pipeline_commands(node);
	index = 0;
	if (cmd_count <= 0)
		return (1);
	if (allocate_pipeline_resources(cmd_count, commands, pipes, pids) != 0)
		return (1);
	*pipes_created = create_all_pipes(*pipes, cmd_count);
	if (*pipes_created != cmd_count - 1)
	{
		cleanup_pipeline_resources(*commands, *pipes, *pids, *pipes_created);
		return (1);
	}
	extract_pipeline_commands(node, *commands, &index);
	if (process_all_heredocs(*commands, cmd_count, shell) == 130)
	{
		cleanup_pipeline_resources(*commands, *pipes, *pids, *pipes_created);
		return (130);
	}
	return (0);
}

int create_pipeline_processes(t_ast **commands, int **pipes, pid_t *pids,
							  int cmd_count, t_env *env, t_shell *shell)
{
	int i;

	initialize_pipeline_pids(pids, cmd_count);
	i = 0;
	while (i < cmd_count)
	{
		pids[i] = fork();
		if (pids[i] < 0)
		{
			perror("minishell: fork");
			terminate_child_processes(pids, i);
			return (1);
		}
		if (pids[i] == 0)
			execute_pipeline_child(commands, pipes, pids, cmd_count, i, env, shell);
		i++;
	}
	return (0);
}

int wait_for_all_processes(pid_t *pids, int cmd_count)
{
	int i;
	int status;
	int final_status;

	i = 0;
	final_status = 0;
	while (i < cmd_count)
	{
		waitpid(pids[i], &status, 0);
		if (i == cmd_count - 1)
			final_status = handle_process_exit_status(status);
		i++;
	}
	return (final_status);
}

void close_parent_pipes(int **pipes, int cmd_count)
{
	int i;

	i = 0;
	while (i < cmd_count - 1)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}
