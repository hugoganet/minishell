/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 17:38:44 by elaudrez          #+#    #+#             */
/*   Updated: 2025/05/22 10:01:23 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
#include "libft.h"

// ----------------------- STRUCTURES --------------

typedef enum s_enum
{
	INPUT,
	OUTPUT,
	HEREDOC,
	WORD,
	PIPE,
	REDIR_APPEND,
} t_enum;

typedef struct s_list
{
	t_enum	type;
	char *str;
	struct s_list	*next;
} t_list;

// ----------------------- FUNCTIONS ---------------

void	free_tab(char **str);

#endif