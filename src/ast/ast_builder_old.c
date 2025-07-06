/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_builder.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 19:16:30 by elaudrez          #+#    #+#             */
/*   Updated: 2025/07/06 10:45:00 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ast.h"

/**
 * @brief Compte le nombre d'arguments ARG après un nœud CMD.
 *
 * @param node Le nœud CMD de départ
 * @return Le nombre d'arguments trouvés
 */
static int count_args(t_token *node)
{
	int count;
	t_token *ptr;

	count = 0;
	ptr = node->next;
	while (ptr && ptr->type != CMD && ptr->type != PIPE)
	{
		if (ptr->type == ARG)
			count++;
		ptr = ptr->next;
	}
	return (count);
}

/**
 * @brief Libère le tableau d'arguments en cas d'erreur d'allocation.
 *
 * @param args Tableau d'arguments à libérer
 * @param index Index jusqu'auquel libérer les éléments
 */
static void free_args_on_error(char **args, int index)
{
	while (--index >= 0)
		free(args[index]);
	free(args);
}

/**
 * @brief Remplit le tableau d'arguments avec la commande et ses arguments.
 *
 * @param node Le nœud CMD de départ
 * @param new_ast Le nœud AST à remplir
 * @return 0 en cas de succès, 1 en cas d'erreur
 */
static int populate_args_array(t_token *node, t_ast *new_ast)
{
	int j;
	t_token *ptr;

	j = 0;
	ptr = node;
	new_ast->args[j] = ft_strdup(ptr->str);
	if (!new_ast->args[j])
		return (1);
	j++;
	ptr = ptr->next;
	while (ptr && ptr->type != CMD && ptr->type != PIPE)
	{
		if (ptr->type == ARG)
		{
			new_ast->args[j] = ft_strdup(ptr->str);
			if (!new_ast->args[j])
			{
				free_args_on_error(new_ast->args, j);
				new_ast->args = NULL;
				return (1);
			}
			j++;
		}
		ptr = ptr->next;
	}
	new_ast->args[j] = NULL;
	return (0);
}

/**
 * @brief Remplit le tableau d'arguments du nœud AST avec les tokens.
 *
 * @param node Le nœud token CMD de départ
 * @param new_ast Le nœud AST à remplir avec les arguments
 */
void fill_args(t_token *node, t_ast *new_ast)
{
	int arg_count;

	arg_count = count_args(node);
	new_ast->args = ft_calloc((arg_count + 2), sizeof(char *));
	if (!new_ast->args)
		return;
	if (populate_args_array(node, new_ast) != 0)
		return;
}

t_ast *cmd_new_ast_node(t_token *node)
{
	t_ast *new_ast;
	// int	i = 0;

	if (!node)
		return (NULL);
	new_ast = ft_calloc(sizeof(t_ast), 1);
	if (!new_ast)
		return (NULL);
	new_ast->type = node->type;
	new_ast->str = node->str;
	fill_args(node, new_ast);
	return (new_ast);
}

t_ast *new_ast_node(t_token *node)
{
	t_ast *new_ast;

	if (!node)
		return (NULL);
	new_ast = ft_calloc(sizeof(t_ast), 1);
	if (!new_ast)
		return (NULL);
	new_ast->type = node->type;
	new_ast->str = node->str;
	return (new_ast);
}

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

/*Fonction pour trouver sur quel token il faut creer un noeud d'ast en calculant leur priorite */
t_token *token_to_split(t_token *node, t_token *end)
{
	int current_priority;
	int lowest_priority;
	t_token *ptr;
	t_token *to_split;

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
		if ((current_priority < lowest_priority) && current_priority < 4)
		{
			to_split = ptr;
			lowest_priority = current_priority;
		}
		ptr = ptr->next;
	}
	return (to_split);
}

/*Fonction recursive qui creer les noeuds de l'ast*/
t_ast *spliter(t_token *node, t_token *end)
{
	t_ast *node_ast;
	t_token *to_split;

	to_split = NULL;
	node_ast = NULL;
	// Si le nœud est NULL ou si c'est le dernier nœud, on retourne NULL
	if (!node || node == end)
		return (NULL);
	to_split = token_to_split(node, end); // Recuperer le token a partir duquel il faut creer un nouveau noeuds (d'abord les | puis les cmds)
	if (!to_split)
		return (NULL);
	if (to_split->type == CMD) // Si cmd, fonction qui va creer un noeud et ranger les arguments a la suite
		node_ast = cmd_new_ast_node(to_split);
	else if (to_split->type == PIPE || is_redirection(to_split->type))
		node_ast = new_ast_node(to_split);
	node_ast->left = spliter(node, to_split);		// Appel recursif pour continuer a creer les noeuds dans les branches qui viennent d'etre creees. Du debut de la chaine au token qui vient d'etre splite
	node_ast->right = spliter(to_split->next, end); // Du token qui vient d'etre split a la fin de la chaine.
	if (!node_ast)
		return (NULL);
	return (node_ast);
}

/**
 * @brief Fonction principale pour construire l'AST à partir d'une liste de tokens.
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
