/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 09:54:12 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/03 10:19:21 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Initialise le pipe pour le heredoc.
 *
 * @param pipefd Tableau des descripteurs de pipe
 * @return 0 en cas de succès, 1 en cas d'erreur
 */
int init_heredoc_pipe(int pipefd[2])
{
	if (pipe(pipefd) == -1)
	{
		perror("minishell: heredoc pipe");
		return (1);
	}
	return (0);
}

/**
 * @brief Valide le token du heredoc.
 *
 * @param token_str Le token complet du heredoc
 * @param pipefd Tableau des descripteurs de pipe (pour nettoyage si erreur)
 * @return 0 en cas de succès, 1 en cas d'erreur
 */
int validate_heredoc_token(char *token_str, int pipefd[2])
{
	if (ft_strlen(token_str) <= 2)
	{
		perror("minishell: heredoc: missing delimiter");
		close(pipefd[0]);
		close(pipefd[1]);
		return (1);
	}
	return (0);
}

/**
 * @brief Vérifie si la ligne correspond au délimiteur.
 *
 * @param line La ligne lue
 * @param delimiter_clean Le délimiteur nettoyé
 * @return 1 si la ligne correspond au délimiteur, 0 sinon
 */
int is_delimiter_line(char *line, char *delimiter_clean)
{
	int delim_len;

	if (!line || !delimiter_clean)
		return (0);
	delim_len = ft_strlen(delimiter_clean);
	if (ft_strncmp(line, delimiter_clean, delim_len) == 0 && line[delim_len] == '\n')
		return (1);
	return (0);
}