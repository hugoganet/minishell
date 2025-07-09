/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_builder.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 19:16:30 by elaudrez          #+#    #+#             */
/*   Updated: 2025/07/09 18:23:14 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Remplit le tableau d'arguments avec la commande et ses arguments.
 *
 * @param node Le nœud CMD de départ
 * @param new_ast Le nœud AST à remplir
 * @return 0 en cas de succès, 1 en cas d'erreur
 */
static int	populate_args_array(t_token *node, t_ast *new_ast)
{
	int		j;
	t_token	*ptr;

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
			if (is_arg(ptr, new_ast, &j))
				return (1);
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
void	fill_args(t_token *node, t_ast *new_ast)
{
	int	arg_count;

	arg_count = count_args(node);
	new_ast->args = ft_calloc((arg_count + 2), sizeof(char *));
	if (!new_ast->args)
		return ;
	if (populate_args_array(node, new_ast) != 0)
		return ;
}

/**
 * @brief Crée un nouveau nœud AST simple (sans arguments).
 *
 * @param node Le token source
 * @return Un pointeur vers le nouveau nœud AST, ou NULL en cas d'erreur
 */
t_ast	*new_ast_node(t_token *node)
{
	t_ast	*new_ast;

	if (!node)
		return (NULL);
	new_ast = ft_calloc(sizeof(t_ast), 1);
	if (!new_ast)
		return (NULL);
	new_ast->type = node->type;
	new_ast->str = node->str;
	return (new_ast);
}

/**
 * @brief Crée un nouveau nœud AST pour une commande avec ses arguments.
 *
 * @param node Le token CMD source
 * @return Un pointeur vers le nouveau nœud AST, ou NULL en cas d'erreur
 */
t_ast	*cmd_new_ast_node(t_token *node)
{
	t_ast	*new_ast;

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
