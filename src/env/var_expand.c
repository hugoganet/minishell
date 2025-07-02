/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expand.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 19:54:08 by elaudrez          #+#    #+#             */
/*   Updated: 2025/07/02 14:59:37 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

	i = 0;
	while (str[i])
	{
		// Si on trouve un `$`, vérifier si c'est suivi de `?`
		if (str[i] == '$' && str[i + 1] == '?')
			break;
		i++;
	}
	// Si on n'a pas trouvé de `$?`, retourner NULL
	if (!str[i] || str[i] != '$' || str[i + 1] != '?')
		return (NULL);
	// On a trouvé `$?`, on met à jour les indices
	// `start` pointe sur le `$`, `end` pointe après le `?`
	*start = i;
	*end = i + 2; // $?
	// Convertir l'entier en chaîne
	return (ft_itoa(data->last_exit_status));
}

static void expand_one_arg(char **arg, t_shell *data)
{
	char *tmp;
	int j;

	j = 0;
	while ((*arg)[j])
	{
		if ((*arg)[j] == '$' && (*arg)[j + 1] == '\"')
		{
			tmp = ft_substr(*arg, 2, ft_strlen(*arg) - 3);
			if (tmp)
			{
				free(*arg);
				*arg = tmp;
			}
			return;
		}
		if ((*arg)[j] == '$')
		{
			if (j > 1 && (*arg)[j - 1] != '\\')
				break;
			tmp = join_str(ft_strdup(*arg), data);
			if (tmp)
			{
				free(*arg);
				*arg = tmp;
			}
			break;
		}
		j++;
	}
	tmp = remove_quotes(*arg);
	if (tmp)
	{
		free(*arg);
		*arg = tmp;
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
