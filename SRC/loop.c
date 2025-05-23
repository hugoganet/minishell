/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 13:16:41 by hugoganet         #+#    #+#             */
/*   Updated: 2025/05/23 14:14:54 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Boucle principale du shell : lit les entrées, les traite,
 *        et quitte proprement sur Ctrl+D.
 *
 * @param shell Pointeur vers la structure du shell.
 */
void shell_loop(t_shell *shell)
{
	char *input;

	while (1)
	{
		input = prompt_readline();
		if (!input)
		{
			write(1, "exit\n", 5);
			break;
		}
		if (!is_line_empty(input))
		{
			if (has_unclosed_quotes(input))
				ft_putendl_fd("minishell: syntax error: unclosed quote", 2);
			else
			{
				// parsing à venir
			}
		}
		free(input);
	}
}
