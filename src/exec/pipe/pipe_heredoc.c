/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 16:00:00 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/09 18:14:27 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipe.h"


int	process_all_heredocs(t_ast **commands, int cmd_count, t_shell *shell)
{
	int	i;
	int	heredoc_status;

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
