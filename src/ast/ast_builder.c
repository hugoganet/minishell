/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_builder.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 19:16:30 by elaudrez          #+#    #+#             */
/*   Updated: 2025/06/09 16:26:43 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int	token_priority(t_token_type type)
{
	if (type == PIPE)
		return (1);
	else if (type == CMD);
		return (2);
	return (3);
}

void	fill_args(t_token *node, t_ast *new_ast) // Remplir avec les arguments le tab du noeud AST
{
	int		i;
	int		j;
	t_token *ptr;
	
	i = 0;
	j = 0;
	ptr = node;
	if (ptr->next != NULL) //etre sur que pas une commande seule ex : ls
	{
		while (ptr && ptr->type != PIPE && ptr->type != CMD)
		{
			ptr = ptr->next;
			i++;
		}
		new_ast->args = malloc((i + 1) * sizeof(char *));
		if (!new_ast->args)
			return NULL;
		i = 0;
		while (j < i)
		{
			new_ast->args[i] = ft_strdup(ptr->str);
			if (!new_ast->args[j])
				return NULL;
			i++;
			j++;
			ptr = ptr->next;
		}
	}	
}

void	add_redir(t_token *node)
{
	
}

t_ast	*cmd_new_ast_node(t_token *node)
{
	t_ast	*new_ast;
	
	if (!node)
		return (NULL);
		
	// Utiliser ft_calloc pour initialiser le contenu du bloc à NULL
	new_ast = ft_calloc(sizeof(t_ast), 1);
	if (!new_ast)
		return (NULL);

	new_ast->type = node->type;
	new_ast->str = node->str;
	fill_args(node, new_ast);
	if ()
	return (new_ast);
}

t_ast	*pipe_new_ast_node(t_token *node)
{
	t_ast	*new_ast;

	if (!node)
		return (NULL);
	// Utiliser ft_calloc pour initialiser le contenu du bloc à NULL
	new_ast = ft_calloc(sizeof(t_ast), 1);
	if (!new_ast)
		return (NULL);
	new_ast->type = node->type;
	new_ast->str = node->str;
	return (new_ast);
}

t_token	*token_to_split(t_token *node, t_token *end)
{
	int		current_priority;
	int		lowest_priority;
	t_token	*ptr;
	t_token	*to_split;

	// On initialise `to_split` à NULL pour éviter les comportements indéfinis.
	// Si aucun token de priorité inférieure à 4 n'est trouvé, on retourne NULL.
	to_split = NULL;
	current_priority = 3;
	lowest_priority = 3;
	ptr = node;
	while (ptr && ptr != end)
	{
		current_priority = token_priority(ptr->type);
		// On check si la priorité courante est strictement inférieur à la plus basse
		if ((current_priority < lowest_priority) && current_priority < 3)
		{
			to_split = ptr;
			lowest_priority = current_priority;
		}
		ptr = ptr->next;
	}
	return (to_split);
}

t_ast	*spliter(t_token *node, t_token *end)
{
	t_ast	*node_ast;
	t_token	*to_split;
	t_token	*ptr;

	to_split = NULL;
	// Si le nœud est NULL ou si c'est le dernier nœud, on retourne NULL
	if (!node || node == end)
		return (NULL);
	to_split = token_to_split(node, end);
	if (!to_split)
		return (NULL);
	if (to_split->type == CMD|| to_split->type == PIPE) //to_split est toujours suppose etre CMD ou PIPE, seuls types qui creer des ast_nodes
	{
		if (to_split->type == CMD)
			node_ast = cmd_new_ast_node(to_split);
		else if (to_split->type == PIPE)
			node_ast = pipe_new_ast_node(to_split);
		node_ast->left = spliter(node, to_split);
		node_ast->right = spliter(to_split->next, end);
	}
	if (!node_ast)
		return (NULL);
	return (node_ast);
}

void	add_redir(t_node)
{
	
}


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