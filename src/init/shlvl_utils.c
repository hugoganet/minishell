/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shlvl_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 21:26:11 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/07 21:26:11 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Vérifie si SHLVL existe dans l'environnement envp
 * @param envp Le tableau d'environnement
 * @return int 1 si SHLVL existe, 0 sinon
 */
int	shlvl_exists_in_envp(char **envp)
{
	int	i;

	if (!envp)
		return (0);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "SHLVL=", 6) == 0)
			return (1);
		i++;
	}
	return (0);
}

/**
 * @brief S'assure que SHLVL est défini à 1 (pour le shell racine)
 * @param env_list La liste d'environnement
 * @return int 0 en cas de succès, 1 en cas d'erreur
 */
int	ensure_shlvl_is_one(t_env *env_list)
{
	t_env	*shlvl_node;
	char	*new_value;

	shlvl_node = env_list;
	while (shlvl_node)
	{
		if (ft_strcmp(shlvl_node->key, "SHLVL") == 0)
			break ;
		shlvl_node = shlvl_node->next;
	}
	if (!shlvl_node)
	{
		shlvl_node = create_env_pair("SHLVL", "1");
		if (!shlvl_node)
			return (1);
		shlvl_node->next = env_list->next;
		env_list->next = shlvl_node;
		return (0);
	}
	new_value = ft_strdup("1");
	if (!new_value)
		return (1);
	free(shlvl_node->value);
	shlvl_node->value = new_value;
	return (0);
}

/**
 * @brief Configure SHLVL selon l'environnement
 * @param envp Environnement système
 * @param env_list Liste d'environnement
 */
void	configure_shlvl(char **envp, t_env *env_list)
{
	if (!envp || !envp[0])
		return ;
	if (shlvl_exists_in_envp(envp))
	{
		if (increment_shlvl(env_list) != 0)
			ft_putendl_fd("minishell: warning: failed to increment SHLVL", 2);
	}
	else
	{
		if (ensure_shlvl_is_one(env_list) != 0)
			ft_putendl_fd("minishell: warning: failed to set SHLVL", 2);
	}
}
