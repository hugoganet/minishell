/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_builder.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 19:16:30 by elaudrez          #+#    #+#             */
/*   Updated: 2025/06/04 15:36:52 by hugoganet        ###   ########.fr       */
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
 * - Les redirections (`REDIR_APPEND`, `REDIR_INPUT`, `REDIR_OUTPUT`, `HEREDOC`) 
 *  ont une priorité intermédiaire (2).
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
	
	// Utiliser ft_calloc pour initialiser les pointeurs left et right à NULL
	new_ast = ft_calloc(sizeof(t_ast), 1);
	if (!new_ast)
		return (NULL);
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
 * @brief Gère les tokens de faible priorité (CMD, ARG, FILES) en construisant une chaîne linéaire.
 *
 * Cette fonction prend un token de priorité 3 (`CMD`, `ARG`, `FILES`) et construit une
 * chaîne d’éléments connectés via le champ `right` du nœud AST, en incluant tous les
 * tokens consécutifs de type `ARG` ou `FILES` qui suivent immédiatement.
 *
 * Cela permet de modéliser correctement des commandes comme :
 *   echo hello world > out.txt
 * Avec un AST de la forme :
 *   CMD "echo"
 *     └── ARG "hello"
 *         └── ARG "world"
 *         └── FILES "out.txt"
 *
 * @param to_split Pointeur vers le token courant (`CMD`, `ARG` ou `FILES`)
 * @param node_ast Nœud AST initial créé à partir de `to_split`
 * @return `t_ast*` Racine de la chaîne AST construite à droite
 */
static t_ast *handle_low_priority(t_token *to_split, t_ast *node_ast)
{
	t_token *next;
	t_ast *curr;
	t_ast *next_node;

	// On initialise le token suivant à celui après to_split
	next = to_split->next;
	// Le pointeur courant AST commence sur le noeud racine (CMD, ARG ou FILES)
	curr = node_ast;
	// Tant qu'on rencontre un ARG ou FILES, on les ajoute en chaîne à droite
	while (next && (next->type == ARG || next->type == FILES))
	{
		// Création d’un nouveau nœud AST à partir du token
		next_node = new_ast_node(next);
		// Si erreur d’allocation, on interrompt proprement la boucle
		if (!next_node)
			break;
		// On rattache le nouveau nœud à droite du courant
		curr->right = next_node;
		// Le nouveau nœud devient le courant pour la prochaine itération
		curr = curr->right;
		// On passe au token suivant
		next = next->next;
	}
	// On retourne la racine du sous-arbre (inchangée)
	return (node_ast);
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
	
	to_split = token_to_split(node, end);
		
	node_ast = new_ast_node(to_split);

	// Si le token à diviser est de priorité 3
	if (token_priority(to_split->type) == 3)
		return (handle_low_priority(to_split, node_ast));

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

	//printf("\nnode->str = %s\n", node->str);
	// à ce niveau, on récupère bien le premier token
	new_ast = spliter(node, NULL);
	return (new_ast);
}