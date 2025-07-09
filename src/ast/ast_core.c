/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_core.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 10:45:00 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/09 12:00:26 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ast.h"

/**
 * @brief Détermine la priorité d'un type de token pour la construction
 * de l'AST.
 *
 * @param type Le type de token à évaluer
 * @return La priorité du token (plus le nombre est bas, plus la priorité
 * est haute)
 */
int token_priority(t_token_type type)
{
	if (type == PIPE)
		return (1);
	else if (is_redirection(type))
		return (2);
	else if (type == CMD)
		return (3);
	return (4);
}

/**
 * @brief Trouve le token de plus faible priorité pour diviser l'AST.
 *
 * Pour les pipes, on trouve le dernier (le plus à droite) pour créer
 * une associativité à droite nécessaire pour l'exécution correcte.
 *
 * @param node Le nœud de départ
 * @param end Le nœud de fin (exclus)
 * @return Le token à utiliser pour diviser, ou NULL si aucun trouvé
 */
t_token *token_to_split(t_token *node, t_token *end)
{
	int current_priority;
	int lowest_priority;
	t_token *ptr;
	t_token *to_split;

	to_split = NULL;
	// On initialise la priorité la plus basse à 4 (aucun token trouvé).
	// Cela permet de trouver le premier token valide.
	current_priority = 4;
	lowest_priority = 4;
	ptr = node;
	// On parcourt la liste de tokens jusqu'à la fin ou jusqu'à
	// atteindre le nœud de fin.
	while (ptr && ptr != end)
	{
		// On calcule la priorité du token actuel.
		current_priority = token_priority(ptr->type);
		// Si le token actuel a une priorité plus basse que la
		// plus basse trouvée jusqu'à présent, et qu'il est inférieur à 4,
		// on le marque comme le token à diviser.
		if (current_priority < lowest_priority && current_priority < 4)
		{
			to_split = ptr;
			lowest_priority = current_priority;
		}
		// Sinon si le token actuel a la même priorité que la plus basse
		// trouvée, et qu'il est égal à 1, on le marque aussi comme
		// token à diviser.
		// ! Cela permet de gérer les pipes correctement en prenant le dernier pipe rencontré.
		else if (current_priority == lowest_priority && current_priority == 1)
			to_split = ptr;
		ptr = ptr->next;
	}
	return (to_split);
}

/**
 * @brief Fonction récursive qui crée les nœuds de l'AST.
 *
 * @param node Le nœud de départ de la liste de tokens
 * @param end Le nœud de fin (exclus) de la liste de tokens
 * @return Un pointeur vers le nœud AST créé, ou NULL en cas d'erreur
 */
t_ast *spliter(t_token *node, t_token *end)
{
	t_ast *node_ast;
	t_token *to_split;

	if (!node || node == end)
		return (NULL);
	// On cherche le token de plus faible priorité pour diviser l'AST.
	to_split = token_to_split(node, end);
	if (!to_split)
		return (NULL);
	// Si le token à diviser est une commande,
	// on créé le noeud et on appelle la fonction fill_args().
	if (to_split->type == CMD)
		node_ast = cmd_new_ast_node(to_split);
	// Sinon, si le token à diviser est un pipe ou une redirection,
	// on créé un nouveau nœud AST avec le token, pas d'appelle à fill_args().
	else if (to_split->type == PIPE || is_redirection(to_split->type))
		node_ast = new_ast_node(to_split);
	else
		return (NULL);
	if (!node_ast)
		return (NULL);
	// On divise la liste de tokens en deux parties :
	// - la partie gauche contenant les tokens avant le token de division
	// - la partie droite contenant les tokens après le token de division
	node_ast->left = spliter(node, to_split);
	node_ast->right = spliter(to_split->next, end);
	return (node_ast);
}

/**
 * @brief Fonction principale pour construire l'AST à partir d'une
 * liste de tokens.
 *
 * Cette fonction est le point d'entrée pour la construction de l'AST.
 * Elle utilise la fonction `spliter` pour diviser les tokens en nœuds AST
 * en fonction de leur priorité.
 *
 * @param node La liste de tokens à partir de laquelle construire l'AST
 * @return Un pointeur vers le nœud racine de l'AST, ou NULL en cas d'erreur
 */
t_ast *build_ast(t_token *node)
{
	t_ast *new_ast;

	// Appel de la fonction récursive pour diviser les tokens
	// et retourne la tête de liste de l'AST.
	new_ast = spliter(node, NULL);
	if (!new_ast)
	{
		free_ast(new_ast);
		return (NULL);
	}
	return (new_ast);
}
