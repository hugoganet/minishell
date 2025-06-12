/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 15:27:47 by elaudrez          #+#    #+#             */
/*   Updated: 2025/06/11 17:33:02 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	quote_flag()

void	handle_heredoc(t_ast *ast_node)
{	
	int		flag;
	int		i;
	char	*line;
	char	*tmp;
	char	*gnl;
	int		fd;
	

	fd = open(STDIN_FILENO, O_RDONLY);
	if (fd == -1)
		exit_error;
	gnl = get_next_line(fd); 
	while (1)
	{
		tmp = ft_strjoin(line, gnl);
		free(line);
		free(gnl);
		line = tmp;
		gnl = get_next_line(fd);
	}
	close(fd);
	free(line);
	
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
	
	Creation fichier tmp
	input tmp terminal 
	
	si flag = 1, alors pas expansion

	if << creation fichier tmp
	
	gnl 
	while (1)
	{
		
	}
	str = gnl;
	if strcmp()limiteur 
	ou que \n strmcp();
	break;
}