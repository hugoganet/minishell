/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 10:45:34 by hugoganet         #+#    #+#             */
/*   Updated: 2025/06/05 11:15:16 by hugoganet        ###   ########.fr       */
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

	// Tant qu'il y a des tokens dans la liste
	while (head)
	{
		// Sauvegarde le pointeur vers le prochain token
		tmp = head->next;
		// Libère la valeur du token
		free(head->str);
		// Libère le token lui-même
		free(head);
		// Avance au prochain token
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
 * Cette fonction libère chaque nœud de l'AST, y compris ses sous-arbres gauche et droit.
 *
 * @param node Pointeur vers la racine de l'AST à libérer.
 */
void free_ast(t_ast *node)
{
	if (!node)
		return;
	// Libération récursive des branches
	free_ast(node->left);
	free_ast(node->right);
	free(node);
}