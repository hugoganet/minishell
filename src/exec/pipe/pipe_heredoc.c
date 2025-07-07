#include "pipe.h"

// setup_heredoc_redirection si spécifique pipe

int process_all_heredocs(t_ast **commands, int cmd_count, t_shell *shell)
{
	int i;
	int heredoc_status;

	i = 0;
	while (i < cmd_count)
	{
		heredoc_status = process_heredocs(commands[i], shell);
		if (heredoc_status == 130)
			return (130);
		i++;
	}
	return (0);
}
