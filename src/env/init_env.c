/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 14:44:49 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/08 16:56:53 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Initialise un nœud de la liste d'environnement.
 *
 * Cette fonction sépare la clé et la valeur d'une variable
 * d'environnement et les stocke dans le nœud.
 *
 * @param node Pointeur vers le nœud à remplir (déjà alloué).
 * @param env_var Chaîne de type "KEY=VALUE".
 * @return int 0 en cas de succès, 1 en cas d’erreur d’allocation.
 */
int	init_env_list_node(t_env *node, char *env_var)
{
	char	*equal;

	if (!node || !env_var)
		return (1);
	equal = ft_strchr(env_var, '=');
	if (equal)
	{
		node->key = ft_substr(env_var, 0, equal - env_var);
		node->value = ft_strdup(equal + 1);
	}
	else
	{
		node->key = ft_strdup(env_var);
		node->value = NULL;
	}
	if (!node->key || (equal && !node->value))
		return (1);
	return (0);
}

/**
 * @brief Crée un nœud t_env à partir d'une chaîne "KEY=VALUE".
 *
 * @param env_var Variable d’environnement au format "KEY=VALUE"
 * @return t_env* Nœud initialisé ou NULL en cas d’erreur
 */
static t_env	*create_env_node(char *env_var)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->next = NULL;
	if (init_env_list_node(node, env_var) != 0)
	{
		free(node->key);
		free(node->value);
		free(node);
		return (NULL);
	}
	return (node);
}

/**
 * @brief Crée une paire clé/valeur sous forme de t_env.
 *
 * Alloue et initialise un nouveau nœud contenant une clé et une valeur.
 *
 * @param key La clé de la variable (ex : "PWD")
 * @param value La valeur associée (ex : "/home/user")
 * @return t_env* Le nœud alloué ou NULL en cas d’échec
 */
t_env	*create_env_pair(const char *key, const char *value)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = ft_strdup(key);
	if (value)
		node->value = ft_strdup(value);
	else
		node->value = NULL;
	node->next = NULL;
	if (!node->key || (value && !node->value))
	{
		free(node->key);
		free(node->value);
		free(node);
		return (NULL);
	}
	return (node);
}

/**
 * @brief Initialise une env minimale avec SHLVL=1 et PWD=<répertoire courant>.
 *
 * Utilisée si l’environnement est vide (env -i).
 *
 * @return t_env* La liste minimale ou NULL en cas d’erreur d’allocation
 */
static t_env	*init_minimal_env(void)
{
	t_env	*head;
	t_env	*pwd;
	char	cwd[PATH_MAX];

	head = create_env_pair("SHLVL", "1");
	if (!head)
		return (NULL);
	if (getcwd(cwd, sizeof(cwd)))
	{
		pwd = create_env_pair("PWD", cwd);
		if (!pwd)
		{
			free_env_list(head);
			return (NULL);
		}
		head->next = pwd;
	}
	return (head);
}

/**
 * @brief Initialise la liste chaînée d’environnement à partir de envp.
 *
 * Si envp est vide ou NULL (ex : env -i), on crée une env minimale contenant
 * SHLVL=1 et PWD=<répertoire courant>.
 *
 * @param envp Le tableau de chaînes "KEY=VALUE" fourni au main
 * @return t_env* La tête de la liste chaînée ou NULL en cas d’échec
 */
t_env	*init_env_list(char **envp)
{
	t_env	*head;
	t_env	*node;
	t_env	*last;

	// Si pas d'env (env -i)
	if (!envp || !envp[0])
		return (init_minimal_env());
	head = NULL;
	last = NULL;
	while (*envp)
	{
		node = create_env_node(*envp++);
		if (!node)
		{
			free_env_list(head);
			return (NULL);
		}
		if (!head)
			head = node;
		else
			last->next = node;
		last = node;
	}
	return (head);
}
