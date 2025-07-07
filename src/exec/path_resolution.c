/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_resolution.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 17:40:32 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/04 09:06:22 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"

static char	*ft_strjoin_three(char *s1, char *s2, char *s3)
{
	char	*tmp;
	char	*res;

	tmp = ft_strjoin(s1, s2);
	if (!tmp)
		return (NULL);
	res = ft_strjoin(tmp, s3);
	free(tmp);
	return (res);
}

static char	*check_direct_path(char *cmd_name)
{
	if (ft_strchr(cmd_name, '/'))
	{
		if (access(cmd_name, X_OK) == 0)
			return (ft_strdup(cmd_name));
		return (NULL);
	}
	return (NULL);
}

static char	*search_in_path_dir(char *path_dir, char *cmd_name)
{
	char	*full_path;

	full_path = ft_strjoin_three(path_dir, "/", cmd_name);
	if (!full_path)
		return (NULL);
	if (access(full_path, X_OK) == 0)
		return (full_path);
	free(full_path);
	return (NULL);
}

static char	*search_in_path_dirs(char **paths, char *cmd_name)
{
	char	*result;
	int		i;

	i = 0;
	while (paths[i])
	{
		result = search_in_path_dir(paths[i], cmd_name);
		if (result)
		{
			free_split(paths);
			return (result);
		}
		i++;
	}
	free_split(paths);
	return (NULL);
}

char	*resolve_command_path(char *cmd_name, t_env *env)
{
	char	**paths;
	char	*path_var;
	char	*direct_result;

	direct_result = check_direct_path(cmd_name);
	if (direct_result)
		return (direct_result);
	if (ft_strchr(cmd_name, '/'))
		return (NULL);
	path_var = get_env_value(env, "PATH");
	if (!path_var)
		return (NULL);
	paths = ft_split(path_var, ':');
	if (!paths)
		return (NULL);
	return (search_in_path_dirs(paths, cmd_name));
}
