/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 17:32:00 by elaudrez          #+#    #+#             */
/*   Updated: 2025/05/30 15:53:06 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_is_sgl_quote(char c)
{
	int	i;

	i = 0;
	if (c == 39)
		i = 1;
	return (i);
}

int	ft_is_dbl_quote(char c)
{
	int	i;

	i = 0;
	if (c == 34)
		i = 1;
	return (i);
}

char	*ft_strcpy(char *dest, char *src)
{
	int	i;
	
	i = 0;
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	**split_path(char **envp)
{
	int		i;
	char	**path;

	i = 0;
	if (!envp)
		return (NULL);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			break ;
		i++;
	}
	if (envp[i] == NULL)
		return (NULL);
	path = ft_split(envp[i] + 5, ':');
	if (!path)
		return (NULL);
	return (path);
}

char	*get_path(char *envp[], char *cmd)
{
	char	**path;
	char	*pathname;
	char	*tmp;
	int		i;

	path = split_path(envp);
	if (!path)
		return (NULL);
	i = 0;
	pathname = NULL;
	while (path[i])
	{
		tmp = ft_strjoin(path[i], "/");
		pathname = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(pathname, X_OK) == 0)
		{
			free_tab(path);
			return (pathname);
		}
		free(pathname);
		i++;
	}
	free_tab(path);
	return (NULL);
}