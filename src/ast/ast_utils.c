/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 16:46:11 by hugoganet         #+#    #+#             */
/*   Updated: 2025/06/23 16:47:20 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Injecte une commande fictive "cat" pour exécuter une redirection isolée.
 *
 * Cette fonction est utilisée lorsque l'utilisateur tape une redirection seule
 * (par exemple `< input.txt | grep`), ce qui est valide en bash. Pour assurer
 * la compatibilité, on crée un nœud AST représentant la commande "cat", qui lit
 * depuis l'entrée standard.
 *
 * @return t_ast* Un pointeur vers le nœud CMD "cat", ou NULL si erreur d’allocation.
 */
t_ast *inject_dummy_cmd(void)
{
	t_ast *new_node;

	// Alloue un nouveau nœud AST
	new_node = ft_calloc(1, sizeof(t_ast));
	if (!new_node)
		return (NULL);
	// Définit le type du nœud comme une commande
	new_node->type = CMD;
	// Affecte la commande "cat" comme chaîne brute
	new_node->str = "cat";
	// Alloue le tableau d’arguments (argv) : ["cat", NULL]
	new_node->args = ft_calloc(2, sizeof(char *));
	if (!new_node->args)
	{
		free(new_node);
		return (NULL);
	}
	// Duplique la chaîne "cat" pour argv[0]
	new_node->args[0] = ft_strdup("cat");
	if (!new_node->args[0])
	{
		free(new_node->args);
		free(new_node);
		return (NULL);
	}
	new_node->args[1] = NULL;
	return (new_node);
}
