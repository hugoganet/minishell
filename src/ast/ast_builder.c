/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_builder.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bernard <bernard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 19:16:30 by elaudrez          #+#    #+#             */
/*   Updated: 2025/06/04 13:12:24 by bernard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	token_priority(t_token_type type)
{
	if (type == PIPE)
		return (1);
	else if (type == REDIR_APPEND || type == REDIR_INPUT || type == REDIR_OUTPUT
		|| type == HEREDOC)
		return (2);
	return (3);
}

t_ast	*new_ast_node(t_token *node)
{
	t_ast	*new_ast;
	
	new_ast = malloc(sizeof(t_ast));
	if (!new_ast)
		return (NULL);
	new_ast->type = node->type;
	new_ast->str = node->str;
	return (new_ast);
}

t_token	*token_to_split(t_token *node, t_token *end)
{
	int	current_priority;
	int	lowest_priority;
	t_token	*ptr;
	t_token	*to_split;

	current_priority = 4;
	lowest_priority = 4;
	ptr = node;
	while (ptr && ptr != end)
	{
		current_priority = token_priority(ptr->type);
		if (current_priority < lowest_priority)
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

	to_split = NULL;
	if (!node || node == end)
		return (NULL);
		
	to_split = token_to_split(node, end);
		
	node_ast = new_ast_node(to_split);
	node_ast->left = spliter(node, to_split);
	
	node_ast->right = spliter(to_split->next, end);
	return (node_ast);
}

t_ast	*build_ast(t_token *node)
{
	t_ast	*new_ast;
	
	new_ast = spliter(node, NULL);
	return (new_ast);
}