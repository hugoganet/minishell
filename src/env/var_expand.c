/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expand.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 19:54:08 by elaudrez          #+#    #+#             */
/*   Updated: 2025/07/01 08:16:28 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Supprime les quotes simples et doubles d’une chaîne tout en respectant les règles du shell.
 *
 * Cette fonction conserve le contenu entre les quotes, mais supprime les caractères
 * de quote eux-mêmes (pas d’expansion entre quotes simples, expansion autorisée dans les doubles).
 *
 * @param str Chaîne originale (non modifiée).
 * @return Une nouvelle chaîne sans les quotes (à libérer après usage).
 */
char *remove_quotes(char *str)
{
	int i;
	int j;
	char *result;
	bool in_single_quote;
	bool in_double_quote;

	if (!str)
		return (NULL);
	result = ft_calloc(sizeof(char), (ft_strlen(str) + 1));
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	in_single_quote = false;
	in_double_quote = false;
	while (str[i])
	{
		// Toggle quote simple si hors quote double
		if (str[i] == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		// Toggle quote double si hors quote simple
		else if (str[i] == '"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		else
			result[j++] = str[i];
		i++;
	}
	return (result);
}

/**
 * @brief Gère l'expansion de la variable $? qui renvoie le statut de sortie de la dernière commande
 *
 * Cette fonction cherche la première occurrence de `$?` dans la chaîne `str`,
 * puis renvoie la valeur du dernier code de sortie sous forme de chaîne allouée dynamiquement.
 * Elle met à jour les indices `start` et `end` pour indiquer la position de `$?` dans la chaîne.
 * Si `$?` n'est pas trouvé, elle retourne NULL.
 *
 * @param str   Chaîne contenant $?
 * @param data  Données du shell contenant last_exit_status
 * @param start Adresse où stocker l'index de début du $?
 * @param end   Adresse où stocker l'index de fin après $?
 * @return Valeur du dernier code de sortie sous forme de chaîne allouée dynamiquement
 */
char *expand_exit_status(char *str, t_shell *data, int *start, int *end)
{
	int i;
	char *exit_status;

	i = 0;
	// Trouver la première occurrence de "$?"
	while (str[i] && (str[i] != '$' && str[i + 1] != '?'))
		i++;
	if (!str[i])
		return (NULL);
	if (str[i] != '$' || str[i + 1] != '?')
		return (NULL);
	*start = i;
	*end = i + 2; // $?
	// Convertir l'entier en chaîne
	exit_status = ft_itoa(data->last_exit_status);
	return (exit_status);
}

static void expand_one_arg(char **arg, t_shell *data)
{
	char *expanded;
	char *unquoted;
	int j;

	j = 0;
	while ((*arg)[j])
	{
		if ((*arg)[j] == '$' && to_exp(*arg))
		{
			expanded = join_str(ft_strdup(*arg), data);
			if (expanded)
			{
				free(*arg);
				*arg = expanded;
			}
			break;
		}
		j++;
	}
	unquoted = remove_quotes(*arg);
	if (unquoted)
	{
		free(*arg);
		*arg = unquoted;
	}
}

/**
 * @brief Applique l’expansion sur tous les arguments d’un nœud AST (CMD uniquement).
 */
void expand_vars(t_ast *node, t_shell *data)
{
	int i;

	if (!node)
		return;
	if (node->type == CMD)
	{
		i = 0;
		while (node->args[i])
			expand_one_arg(&node->args[i++], data);
	}
	expand_vars(node->left, data);
	expand_vars(node->right, data);
}
