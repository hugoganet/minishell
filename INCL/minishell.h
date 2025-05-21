/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 17:38:44 by elaudrez          #+#    #+#             */
/*   Updated: 2025/05/21 12:20:20 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>

typedef enum s_enum
{
	INPUT,
	HEREDOC,
	TRUNC,
	APPEND,
	CMD,
	PIPE,
	ARG,
} t_enum;

typedef struct s_list
{
	t_enum	type;
	char *str;
	struct s_list	*next;
} t_list;



#endif