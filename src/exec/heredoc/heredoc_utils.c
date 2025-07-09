/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 09:54:12 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/09 18:00:28 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"

/**
 * @brief Initialise le pipe pour la gestion des heredocs.
 * 
 * @param pipefd Un tableau de deux entiers pour stocker les
 * descripteurs de fichiers du pipe.
 * @return 0 si l'initialisation réussit, 1 en cas d'erreur.
 */
int	init_heredoc_pipe(int pipefd[2])
{
	if (pipe(pipefd) == -1)
	{
		perror("minishell: heredoc pipe");
		return (1);
	}
	return (0);
}

/**
 * @brief Valide le token de heredoc.
 * 
 * Cette fonction vérifie si le token de heredoc est valide (<<).
 * Elle close les descripteurs de pipe si le token est invalide.
 * 
 * @param token_str Le token de heredoc à valider.
 * @param pipefd Un tableau de deux entiers contenant
 * les descripteurs de fichiers du pipe.
 * @return 0 si le token est valide, 1 si le token est invalide.
 */
int	validate_heredoc_token(char *token_str, int pipefd[2])
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

int	is_delimiter_line(char *line, char *delimiter_clean)
{
	int	delim_len;

	if (!line || !delimiter_clean)
		return (0);
	delim_len = ft_strlen(delimiter_clean);
	if (line[ft_strlen(line) - 1] == '\n')
		line[ft_strlen(line) - 1] = '\0';
	if (ft_strncmp(line, delimiter_clean, delim_len) == 0
		&& line[delim_len] == '\0')
	{
		return (1);
	}
	return (0);
}

void	close_pipe_fds(int pipefd[2])
{
	close(pipefd[0]);
	close(pipefd[1]);
}

int	is_heredoc_delimiter_quoted(const char *delimiter)
{
	int	i;

	if (!delimiter)
		return (0);
	i = 0;
	while (delimiter[i])
	{
		if (delimiter[i] == '\'' || delimiter[i] == '"')
			return (1);
		i++;
	}
	return (0);
}
