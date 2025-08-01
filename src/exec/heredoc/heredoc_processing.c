/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_processing.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 00:00:00 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/09 17:59:45 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"
#include <signal.h>

/**
 * @brief Traite une ligne d'entrée de l'heredoc.
 * 
 * Cette fonction vérifie si la ligne est le délimiteur,
 * gère les signaux, étend la ligne si nécessaire,
 * et écrit la ligne dans le pipe de l'heredoc.
 * 
 * @param line La ligne d'entrée de l'utilisateur.
 * @param delimiter_clean Le délimiteur de l'heredoc sans quotes.
 * @param shell La structure principale du shell.
 * @param pipefd Le descripteur de fichier du pipe de l'heredoc.
 * @return 1 si la ligne a été écrite, 0 si c'est le délimiteur,
 * -1 en cas d'erreur.
 */
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

/**
 * @brief Lit les lignes d'un heredoc jusqu'à ce que le délimiteur
 * soit atteint
 * ou que l'utilisateur interrompe avec Ctrl+C.
 */
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
			return (-1);
		}
		result = process_heredoc_input_line(line, delimiter_clean,
				shell, pipefd);
		if (result <= 0)
			return (result == 0);
	}
}

/**
 * @brief Fonction principale pour traiter un heredoc.
 */
int	process_heredoc_main(t_shell *shell,
		int pipefd[2], char *delimiter_clean)
{
	int	read_result;

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
