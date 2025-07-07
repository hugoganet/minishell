/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 00:00:00 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/07 00:00:00 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"

static char	*extract_filename(char *str)
{
	if (!str)
		return (NULL);
	while (*str == '<' || *str == '>')
		str++;
	while (*str == ' ' || *str == '\t')
		str++;
	return (str);
}

static int	open_redir_file(t_token_type type, char *filename)
{
	int	fd;

	fd = -1;
	if (type == REDIR_INPUT)
		fd = open(filename, O_RDONLY);
	else if (type == REDIR_OUTPUT)
		fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (type == REDIR_APPEND)
		fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
		perror(filename);
	return (fd);
}

static int	apply_single_redirection(t_token_type type, int fd)
{
	int	result;

	result = 0;
	if (type == REDIR_INPUT)
		result = dup2(fd, STDIN_FILENO);
	else
		result = dup2(fd, STDOUT_FILENO);
	if (result == -1)
	{
		perror("dup2");
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

static int	process_redirection_node(t_ast *node)
{
	char	*filename;
	int		fd;

	if (node->type != REDIR_INPUT && node->type != REDIR_OUTPUT
		&& node->type != REDIR_APPEND)
		return (0);
	filename = extract_filename(node->str);
	if (!filename || *filename == '\0')
	{
		ft_putendl_fd("setup_redirections: missing filename", STDERR_FILENO);
		return (1);
	}
	fd = open_redir_file(node->type, filename);
	if (fd == -1)
		return (1);
	return (apply_single_redirection(node->type, fd));
}

int	setup_redirections(t_ast *node)
{
	if (!node || !node->str)
	{
		ft_putendl_fd("setup_redirections: invalid node or str",
			STDERR_FILENO);
		return (1);
	}
	while (node)
	{
		if (process_redirection_node(node) != 0)
			return (1);
		node = node->right;
	}
	return (0);
}
