/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 15:30:08 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/05 19:48:50 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"

static int	process_all_heredocs(t_ast *node, t_shell *shell)
{
	t_ast	*tmp;

	tmp = node;
	while (tmp)
	{
		if (tmp->type == HEREDOC)
		{
			if (handle_heredoc(tmp->str, shell) == 130)
				return (130);
		}
		tmp = tmp->right;
	}
	return (0);
}

int	apply_parent_redirections(t_ast *node, t_shell *shell)
{
	int	saved_stdin;
	int	saved_stdout;
	int	result;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	result = process_all_heredocs(node, shell);
	if (result != 130 && setup_redirections(node) != 0)
		result = 1;
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
	return (result);
}
