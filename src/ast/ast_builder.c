/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_builder.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 19:16:30 by elaudrez          #+#    #+#             */
/*   Updated: 2025/06/05 14:01:29 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Retourne la priorité d'un token en fonction de son type.
 * Cette fonction attribue une priorité aux tokens
 * pour déterminer l'ordre de traitement
 * dans l'arbre de syntaxe abstraite (AST).
 * La priorité est définie comme suit :
 *
 * - `PIPE` a la priorité la plus basse (1).
 *
 * - Les redirections (`<`, `>`, `>>`, `<<`) ont une priorité intermédiaire (2),
 *   donc évaluées avant les pipes mais après les commandes et arguments.
 *
 * - Les autres tokens (comme `WORD`, `CMD`, `ARG`, `FILES`)
 * ont la priorité la plus haute (3).
 * @param type Le type du token dont on veut connaître la priorité.
 * @return `int` La priorité du token.
 */
int	token_priority(t_token_type type)
{
	if (type == PIPE)
		return (1);
	else if (type == REDIR_APPEND || type == REDIR_INPUT
			|| type == REDIR_OUTPUT || type == HEREDOC)
		return (2);
	return (3);
}

/**
 * @brief Crée un nouveau nœud de l'arbre de syntaxe abstraite (AST) à partir d'un token.
 * 
 * Cette fonction alloue de la mémoire pour un nouveau nœud AST,
 * initialise ses champs `type` et `str` à partir du token fourni,
 * et initialise les pointeurs `left` et `right` à NULL.
 * @param node Pointeur vers le token à partir duquel créer le nœud AST.
 * @return `t_ast*` Pointeur vers le nouveau nœud AST créé.
 */
t_ast	*new_ast_node(t_token *node)
{
	t_ast	*new_ast;

	// Si le token est NULL, on retourne NULL
	if (!node)
		return (NULL);
	// Utiliser ft_calloc pour initialiser les pointeurs left et right à NULL
	new_ast = ft_calloc(sizeof(t_ast), 1);
	if (!new_ast)
		return (NULL);
	// On initialise les champs du nœud AST
	new_ast->type = node->type;
	new_ast->str = node->str;
	return (new_ast);
}

/**
 * @brief Trouve le token à diviser dans la liste de tokens.
 * 
 * Cette fonction parcourt la liste de tokens à partir du nœud `node` jusqu'à `end`
 * et cherche le token avec la priorité la plus basse.
 * Elle retourne le token à diviser, qui sera utilisé pour construire l'arbre de syntaxe abstraite (AST).
 * @param node Pointeur vers le premier token de la liste.
 * @param end Pointeur vers le token de fin (NULL si on veut aller jusqu'à la fin de la liste).
 * @return `t_token*` Pointeur vers le token à diviser.
 */
t_token	*token_to_split(t_token *node, t_token *end)
{
	int		current_priority;
	int		lowest_priority;
	t_token	*ptr;
	t_token	*to_split;

	// On initialise `to_split` à NULL pour éviter les comportements indéfinis.
	// Si aucun token de priorité inférieure à 4 n'est trouvé, on retourne NULL.
	to_split = NULL;
	current_priority = 4;
	lowest_priority = 4;
	ptr = node;
	while (ptr && ptr != end)
	{
		current_priority = token_priority(ptr->type);
		// On check si la priorité courante est strictement inférieur à la plus basse
		if (current_priority < lowest_priority)
		{
			to_split = ptr;
			lowest_priority = current_priority;
		}
		ptr = ptr->next;
	}
	return (to_split);
}

/**
 * @brief Construit récursivement l'arbre de syntaxe abstraite (AST) à partir d'une liste de tokens.
 * 
 * Cette fonction prend en entrée un pointeur vers le premier token de la liste
 * et un pointeur vers le token de fin (NULL si on veut aller jusqu'à la fin de la liste).
 * Elle cherche le token à diviser en fonction de sa priorité, puis crée un nœud AST
 * avec ce token. Ensuite, elle divise récursivement la liste de tokens
 * en deux sous-listes :
 * 
 * - La sous-liste gauche contient les tokens avant le token à diviser.
 * 
 * - La sous-liste droite contient les tokens après le token à diviser.
 * @param node Pointeur vers le premier token de la liste.
 * @param end Pointeur vers le token de fin (NULL si on veut aller jusqu'à la fin de la liste).
 * @return `t_ast*` Pointeur vers la racine de l'AST construit.
 */
t_ast	*spliter(t_token *node, t_token *end)
{
	t_ast	*node_ast;
	t_token	*to_split;

	to_split = NULL;
	// Si le nœud est NULL ou si c'est le dernier nœud, on retourne NULL
	if (!node || node == end)
		return (NULL);
	// On trouve le token à diviser en fonction de la priorité
	to_split = token_to_split(node, end);
	// On créé un nouveau noeud à partir du token à diviser
	node_ast = new_ast_node(to_split);
	if (!node_ast)
		return (NULL);
	node_ast->left = spliter(node, to_split);
	node_ast->right = spliter(to_split->next, end);
	return (node_ast);
}

/**
 * @brief Construit l'arbre de syntaxe abstraite (AST) à partir d'une liste de tokens.
 * 
 * Cette fonction prend en entrée un pointeur vers le premier token de la liste
 * et construit un arbre de syntaxe abstraite (AST) en utilisant la fonction `spliter`.
 * 
 * @param node Pointeur vers le premier token de la liste.
 * @return `t_ast*` Pointeur vers la racine de l'AST construit.
 */
t_ast	*build_ast(t_token *node)
{
	t_ast	*new_ast;

	// Point d’entrée unique pour construire l’AST à partir de la liste de tokens
	new_ast = spliter(node, NULL);
	if (!new_ast)
	{
		// Si il y'a un problème, on free tout l'AST récursivement en partant du head
		free_ast(new_ast);
		return (NULL);
	}
	return (new_ast);
}