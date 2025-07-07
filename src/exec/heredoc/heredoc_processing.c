/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_processing.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 00:00:00 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/07 00:00:00 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"
#include <signal.h>

static int	process_heredoc_input_line(char *line, char *delimiter_clean,
		t_shell *shell, int pipefd)
{
	if (!line)
		return (0);
	if (is_delimiter_line(line, delimiter_clean))
	{
		free(line);
		return (0);
	}
	if (g_signal == SIGINT)
	{
		free(line);
		return (-1);
	}
	line = expand_heredoc_line(line, !is_heredoc_delimiter_quoted(
				delimiter_clean), shell);
	if (line)
	{
		write(pipefd, line, ft_strlen(line));
		write(pipefd, "\n", 1);
		free(line);
	}
	return (1);
}

static int	read_heredoc_lines(char *delimiter_clean, t_shell *shell,
		int pipefd)
{
	char	*line;
	int		result;

	while (1)
	{
		printf("> ");
		fflush(stdout);
		line = get_next_line(STDIN_FILENO);
		if (g_signal == SIGINT)
		{
			if (line)
				free(line);
			printf("\n");
			return (0);
		}
		if (!line)
		{
			printf("\n");
			return (0);
		}
		result = process_heredoc_input_line(line, delimiter_clean,
				shell, pipefd);
		if (result <= 0)
			return (result == 0);
	}
}

int	process_heredoc_main(char *token_str, t_shell *shell,
		int pipefd[2], char *delimiter_clean)
{
	int	read_result;

	(void)token_str;
	read_result = read_heredoc_lines(delimiter_clean, shell, pipefd[1]);
	close(pipefd[1]);
	if (read_result == 0)
	{
		close(pipefd[0]);
		g_signal = 0;
		return (130);
	}
	add_heredoc_fd(shell, pipefd[0]);
	return (0);
}
