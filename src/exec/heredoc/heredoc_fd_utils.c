/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_fd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 19:12:44 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/09 17:59:01 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include <unistd.h>

/**
 * @brief Ajoute un descripteur de fichier d'heredoc à la liste des
 * heredoc_fds du shell.
 */
void	add_heredoc_fd(t_shell *shell, int fd)
{
	t_heredoc_fd	*new_fd;
	t_heredoc_fd	*cur;

	if (fd < 0)
		return ;
	new_fd = ft_calloc(sizeof(t_heredoc_fd), 1);
	if (!new_fd)
		return ;
	new_fd->fd = fd;
	new_fd->next = NULL;
	if (!shell->heredoc_fds)
		shell->heredoc_fds = new_fd;
	else
	{
		cur = shell->heredoc_fds;
		while (cur->next)
			cur = cur->next;
		cur->next = new_fd;
	}
}

void	close_all_heredoc_fds(t_shell *shell)
{
	t_heredoc_fd	*cur;

	cur = shell->heredoc_fds;
	while (cur)
	{
		if (cur->fd != -1)
			close(cur->fd);
		cur = cur->next;
	}
}

void	free_all_heredoc_fds(t_shell *shell)
{
	t_heredoc_fd	*cur;
	t_heredoc_fd	*tmp;

	cur = shell->heredoc_fds;
	while (cur)
	{
		tmp = cur->next;
		free(cur);
		cur = tmp;
	}
	shell->heredoc_fds = NULL;
}
