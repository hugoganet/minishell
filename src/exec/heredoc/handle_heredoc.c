/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 15:00:00 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/03 09:58:13 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <signal.h>

/**
 * @brief Traite une ligne du heredoc.
 *
 * @param line La ligne à traiter
 * @param delimiter_clean Le délimiteur nettoyé
 * @param expand_enabled 1 si l'expansion est activée, 0 sinon
 * @param shell Les données du shell
 * @param pipefd Descripteur de pipe en écriture
 * @return 1 pour continuer, 0 pour arrêter, -1 pour signal
 */
static int process_heredoc_input_line(char *line, char *delimiter_clean,
									  int expand_enabled, t_shell *shell, int pipefd)
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
		return (-1); // Retourner -1 pour indiquer une interruption par signal
	}
	line = expand_heredoc_line(line, expand_enabled, shell);
	if (line)
	{
		write(pipefd, line, ft_strlen(line));
		free(line);
	}
	return (1);
}

/**
 * @brief Lit et traite toutes les lignes du heredoc.
 *
 * @param delimiter_clean Le délimiteur nettoyé
 * @param expand_enabled 1 si l'expansion est activée, 0 sinon
 * @param shell Les données du shell
 * @param pipefd Descripteur de pipe en écriture
 * @return 1 si lecture normale, 0 si interrompue par signal
 */
static int read_heredoc_lines(char *delimiter_clean, int expand_enabled,
							  t_shell *shell, int pipefd)
{
	char *line;
	int result;

	while (1)
	{
		line = get_next_line(STDIN_FILENO);
		if (g_signal == SIGINT)
		{
			if (line)
				free(line);
			return (0); // Interruption par signal
		}
		result = process_heredoc_input_line(line, delimiter_clean,
											expand_enabled, shell, pipefd);
		if (result <= 0)
			return (result == 0);
	}
}

/**
 * @brief Gère un heredoc et enregistre le descripteur dans shell->heredoc_fd.
 *
 * Cette fonction lit l'entrée utilisateur jusqu'au délimiteur du heredoc,
 * écrit les lignes dans un pipe, puis conserve l'extrémité lecture du pipe
 * dans shell->heredoc_fd pour une redirection future.
 *
 * @param token_str Le token complet du heredoc (ex: "<<EOF", "<<'END'")
 * @param shell Structure principale du shell contenant les variables d’environnement
 * @return 0 si succès, 130 si interruption SIGINT, 1 si erreur
 */
int handle_heredoc(char *token_str, t_shell *shell)
{
	int pipefd[2], read_result;
	char *delimiter_clean;
	struct sigaction sa_old;

	set_heredoc_sigint(&sa_old);
	if (init_heredoc_pipe(pipefd) != 0 || validate_heredoc_token(token_str, pipefd) != 0)
		return (restore_sigint(&sa_old), 1);
	delimiter_clean = clean_heredoc_delimiter(token_str + 2);
	if (!delimiter_clean)
		return (close_pipe_fds(pipefd), restore_sigint(&sa_old), 1);
	read_result = read_heredoc_lines(delimiter_clean, !is_heredoc_delimiter_quoted(token_str + 2), shell, pipefd[1]);
	free(delimiter_clean);
	close(pipefd[1]);
	restore_sigint(&sa_old);
	if (read_result == 0)
		return (close(pipefd[0]), g_signal = 0, 130);
	shell->heredoc_fd = pipefd[0];
	return (0);
}
