/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 15:27:47 by elaudrez          #+#    #+#             */
/*   Updated: 2025/06/23 11:46:00 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// ----- CODE ELSA -----
/* 
int	quote_flag(void);

void	handle_heredoc(t_ast *ast_node)
{
	char	*line;
	char	*gnl;
	int		pipefd[2];

	if (pipe(pipefd[2]) == -1)
	{
		perror("pipe");
		return (EXIT_FAILURE);
	}
	gnl = get_next_line(0);
	while (1)
	{
		if (!gnl)
			break;
		if (ft_strncmp(line, delim, ft_strlen(delim)) == 0 && line[ft_strlen(delim)] == '\n')
		{
			free(line);
			break;
		}
		write(pipefd[1], line, ft_strlen(line));	
	}
	 close(pipefd[1]);
}
	
	flag = 0;
	i = 0;
	if(ast_node->str[2] == 34)
	{
		flag = 1;
		i = 3;
	}
	else if (ast_node->str[2] == 39 || ft_isalnum(ast_node->str[2]))
		i = 3;
	
	si flag = 1, alors pas expansion
	gnl 
	str = gnl;
	if strcmp()limiteur 
	ou que \n strmcp();
	break;
} */