/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenisation.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 13:28:30 by elaudrez          #+#    #+#             */
/*   Updated: 2025/05/21 14:38:08 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*create_node(char *str, t_enum type)
{
	t_list	*new_node;
	
	new_node = malloc(sizeof(new_node));
	if (!new_node)
		return (NULL);
	new_node->type = type;
	new_node->str = str;
	new_node->next = NULL;
	return (new_node);
}

char	**get_env(char **env)
{
	char	**new_env;
	int		i;

	i = 0;
	while (env[i])
		i++;
	new_env = malloc((i + 1) * sizeof(char *));
	if (!new_env)
		return (NULL);
	i = 0;
	while(env[i])
	{
		new_env[i] = ft_strdup(env[i]);
		i++;
	}
	new_env[i] = NULL;
	return (new_env);
}

int	main(int ac, char *av[], char **env)
{
	(void)ac;
	(void)av;
	int	i;
	char **new_env;

	i = 0;
	char	*rl;
	rl = readline("Minishell > ");
	printf("%s\n", rl);
	new_env = get_env(env);
	
	return (0);
}



