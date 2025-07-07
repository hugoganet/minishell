/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_core.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 15:00:00 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/06 11:58:45 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"
#include <signal.h>

void	setup_heredoc_redirection(t_shell *shell)
{
	t_heredoc_fd	*last_heredoc;
	t_heredoc_fd	*current;

	last_heredoc = NULL;
	current = shell->heredoc_fds;
	while (current)
	{
		if (current->fd != -1)
			last_heredoc = current;
		current = current->next;
	}
	if (last_heredoc && last_heredoc->fd != -1)
	{
		if (dup2(last_heredoc->fd, STDIN_FILENO) == -1)
			perror("minishell: dup2 heredoc");
		close(last_heredoc->fd);
		last_heredoc->fd = -1;
	}
}

int	process_heredocs(t_ast *ast_root, t_shell *shell)
{
	t_ast	*tmp;
	int		heredoc_status;

	tmp = ast_root;
	while (tmp)
	{
		if (tmp->type == HEREDOC)
		{
			heredoc_status = handle_heredoc(tmp->str, shell);
			if (heredoc_status == 130)
			{
				close_all_heredoc_fds(shell);
				free_all_heredoc_fds(shell);
				return (130);
			}
		}
		tmp = tmp->right;
	}
	return (0);
}

int	handle_heredoc(char *token_str, t_shell *shell)
{
	int					pipefd[2];
	char				*delimiter_clean;
	struct sigaction	sa_old;
	int					result;

	set_heredoc_sigint(&sa_old);
	if (init_heredoc_pipe(pipefd) != 0
		|| validate_heredoc_token(token_str, pipefd) != 0)
		return (restore_sigint(&sa_old), 1);
	delimiter_clean = expand_and_clean_delimiter(token_str + 2, shell);
	if (!delimiter_clean)
		return (close_pipe_fds(pipefd), restore_sigint(&sa_old), 1);
	result = process_heredoc_main(token_str, shell, pipefd, delimiter_clean);
	free(delimiter_clean);
	restore_sigint(&sa_old);
	return (result);
}
