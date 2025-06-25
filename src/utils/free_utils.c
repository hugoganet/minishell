/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 10:45:34 by hugoganet         #+#    #+#             */
/*   Updated: 2025/06/17 12:01:23 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

/**
 * @brief Libère toute la liste de tokens allouée par le parsing.
 *
 * @param head Le pointeur vers le premier token de la liste.
 */
void free_token_list(t_token *head)
{
	t_token *tmp;

	while (head)
	{
		tmp = head->next;
		if (head->str)
			free(head->str);
		free(head);
		head = tmp;
	}
}

/**
 * @brief Libère un tableau de chaînes de caractères alloué dynamiquement.
 *
 * @param split Le tableau de chaînes de caractères à libérer.
 */
void free_split(char **split)
{
	int i;

	i = 0;
	while (split && split[i])
		free(split[i++]);
	free(split);
}

/**
 * @brief Libère récursivement un arbre de syntaxe abstraite (AST).
 *
 * Cette fonction libère chaque nœud de l'AST, y compris ses sous-arbres gauche et droit,
 * ainsi que le tableau `args` si présent (dans les nœuds CMD).
 *
 * @param ast Pointeur vers la racine de l'AST à libérer.
 */
void free_ast(t_ast *ast)
{
	int i;

	if (!ast)
		return;
	// Libération récursive
	if (ast->left)
	{
		free_ast(ast->left);
		ast->left = NULL;
	}
	if (ast->right)
	{
		free_ast(ast->right);
		ast->right = NULL;
	}
	// Libération du tableau d’arguments
	if (ast->args)
	{
		i = 0;
		while (ast->args[i])
			free(ast->args[i++]);
		free(ast->args);
		ast->args = NULL;
	}
	free(ast);
}

/**
 * @brief Libère la mémoire allouée pour l'environnement.
 *
 * @param env Le tableau de chaînes alloué à libérer.
 */
void free_env(char **env)
{
	int i = 0;

	// Libère chaque chaîne dans le tableau
	while (env && env[i])
		free(env[i++]);
	// Libère le tableau lui-même
	free(env);
}

/**
 * @brief Libère la mémoire de la structure t_env
 *
 * @param env La liste chaînée d'environnement à libérer.
 */
void free_env_list(t_env *env_list)
{
	t_env *temp;

	while (env_list)
	{
		temp = env_list->next;
		if (env_list->key)
			free(env_list->key);
		if (env_list->value)
			free(env_list->value);
		free(env_list);
		env_list = temp;
	}
}
