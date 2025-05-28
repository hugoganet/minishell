/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_builder.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 10:25:38 by hugoganet         #+#    #+#             */
/*   Updated: 2025/05/28 10:45:25 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ast.h"

/**
 * @brief Crée un nœud AST de type commande.
 *
 * @param tokens Liste de tokens représentant une commande
 * @return t_ast* Nœud AST_CMD
 */
static t_ast *create_command_node(t_token *tokens)
{
	t_ast	*node;
	t_token	*tmp;
	int		count;
	int		i;

	tmp = tokens;
	count = 0;
	while (tmp) // Parcourir la liste de tokens pour compter les mots
	{
		if (tmp->type != TOK_WORD)
			break;
		count++;
		tmp = tmp->next;
	}
	// Si aucun mot n'est trouvé, retourner NULL
	if (count == 0) 
		return (NULL);
	// Allouer de la mémoire pour le nœud AST_CMD et son tableau de commandes
	node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	// Set le type du nœud a AST_CMD
	node->type = AST_CMD;
	// Allouer de la mémoire pour le tableau de commandes
	node->cmd = ft_calloc(sizeof(char *), (count + 1));
	if (!node->cmd)
		return (free(node), NULL);
	i = 0;
	// Remplir le tableau de commandes avec les valeurs des tokens	
	while (i < count)
	{
		// Récupérer la valeur du token courant et l'ajouter au tableau de commandes de l'AST
		node->cmd[i] = ft_strdup(tokens->value);
		// Vérifier si la duplication a échoué
		if (!node->cmd[i])
		{
			// Si échec, libérer la mémoire allouée précédemment
			while (i > 0)
				free(node->cmd[--i]);
			// Libérer le tableau de commandes et le nœud lui-même
			free(node->cmd);
			free(node);
			return (NULL);
		}
		// Passer au token suivant
		tokens = tokens->next;
		// Incrémenter l'index pour le tableau de commandes
		i++;
	}
	// Terminer le tableau de commandes par NULL
	node->left = NULL;
	node->right = NULL;
	return (node);
}