/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 17:32:00 by elaudrez          #+#    #+#             */
/*   Updated: 2025/06/26 14:56:55 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Met à jour l'état si on est dans une double quote ou non
 * @param c Caractère courant
 * @param in_sgl Pointeur vers l'état des single quotes
 * @param in_dbl Pointeur vers l'état des double quotes
 */
void in_dbl(char c, bool *in_sgl, bool *in_dbl)
{
	// Si on n'est pas dans une sgl
	if (c == '"' && *in_sgl == false)
		*in_dbl = !(*in_dbl); // On inverse l'état
}

/**
 * @brief Met à jour l'état si on est dans une single quote ou non
 * @param c Caractère courant
 * @param in_sgl Pointeur vers l'état des single quotes
 * @param in_dbl Pointeur vers l'état des double quotes
 */
void in_sgl(char c, bool *in_sgl, bool *in_dbl)
{
	// Si on n'est pas dans une dbl
	if (c == '\'' && *in_dbl == false)
		*in_sgl = !(*in_sgl); // On inverse l'état
}

/**
 * @brief Vérifie si une variable dans la chaîne doit être expansée par rapport à sa position dans les quotes.
 * 
 * - Entre simple quote, on expanse pas.
 * 
 * - Entre double quote, on expanse.
 * 
 * @param str Chaîne à vérifier
 * @return `int` 1 si expansable, 0 sinon
*/
int to_exp(char *str)
{
	bool is_in_single_quote;
	bool is_in_double_quote;
	int i;

	is_in_single_quote = false;
	is_in_double_quote = false;
	i = 0;
	while (str[i])
	{
		if (str[i] == '\'')
			in_sgl(str[i], &is_in_single_quote, &is_in_double_quote);
		else if (str[i] == '"')
			in_dbl(str[i], &is_in_single_quote, &is_in_double_quote);
		else if (str[i] == '$' && !is_in_single_quote)
			return (1); // Variable à expanser trouvée

		i++;
	}
	return (0); // Aucune variable à expanser
}

char *
ft_strcpy(char *dest, char *src)
{
	int i;

	i = 0;
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

/**
 * @brief Incrémente la variable SHLVL dans l'environnement.
 *
 * Cette fonction recherche la variable d'environnement SHLVL, la convertit
 * en entier, l'incrémente de 1, et met à jour sa valeur dans l'environnement.
 * Si SHLVL n'existe pas, elle est créée avec la valeur "1".
 * Si SHLVL n'est pas un nombre valide ou est négative, elle est réinitialisée à "1".
 * Si SHLVL dépasse 999, elle est réinitialisée à "1" (comportement de Bash).
 *
 * @param env_list La liste chaînée des variables d'environnement
 * @return int 0 en cas de succès, 1 en cas d'erreur
 */
int increment_shlvl(t_env *env_list)
{
	t_env *shlvl_node;
	int level;
	char *new_value;

	// Rechercher le nœud SHLVL dans la liste d'environnement
	shlvl_node = env_list;
	while (shlvl_node)
	{
		if (ft_strcmp(shlvl_node->key, "SHLVL") == 0)
			break;
		shlvl_node = shlvl_node->next;
	}
	// Si SHLVL n'existe pas, créer un nouveau nœud avec la valeur "1"
	if (!shlvl_node)
	{
		shlvl_node = create_env_pair("SHLVL", "1");
		if (!shlvl_node)
			return (1);
		// Ajouter le nouveau nœud à la liste
		shlvl_node->next = env_list->next;
		env_list->next = shlvl_node;
		return (0);
	}
	// Convertir la valeur actuelle en entier
	level = ft_atoi(shlvl_node->value);
	// Si la valeur n'est pas numérique ou est négative, réinitialiser à 1
	if (level < 0)
		level = 0;
	// Incrémenter la valeur
	level++;
	// Si la valeur dépasse 999, la réinitialiser à 1 (comportement de Bash)
	if (level > 999)
		level = 1;
	// Convertir la nouvelle valeur en chaîne de caractères
	new_value = ft_itoa(level);
	if (!new_value)
		return (1);
	// Libérer l'ancienne valeur et mettre à jour
	free(shlvl_node->value);
	shlvl_node->value = new_value;
	return (0);
}
