/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_builder.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 10:25:38 by hugoganet         #+#    #+#             */
/*   Updated: 2025/05/28 11:46:13 by hugoganet        ###   ########.fr       */
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

/**
 * @brief Trouve le token opérateur de plus basse priorité hors parenthèses.
 *
 * @param tokens Liste de tokens
 * @return t_token* Le token opérateur principal, ou NULL si commande simple
 */
static t_token *find_main_operator(t_token *tokens)
{
	int paren_depth;
	t_token *op;

	paren_depth = 0;
	op = NULL;
	// Parcour la liste de tokens
	while (tokens)
	{
		// Gérer la profondeur des parenthèses
		// Si on trouve une parenthèse ouvrante, on incrémente la profondeur
		// Si on trouve une parenthèse fermante, on décrémente la profondeur
		if (tokens->type == TOK_PAREN_LEFT)
			paren_depth++;
		else if (tokens->type == TOK_PAREN_RIGHT)
			paren_depth--;
		// Si on est en dehors des parenthèses
		else if (paren_depth == 0)
		{
			// Si on trouve un opérateur logique '||', on le retourne immédiatement
			// car c'est l'opérateur de plus basse priorité
			if (tokens->type == TOK_OR)
				return (tokens);
			// Si on trouve un opérateur logique '&&', on le garde en mémoire
			// sinon si on trouve un opérateur pipe '|', et qu'il n'y a pas déjà un opérateur logique
			// on le garde en mémoire 
			else
			if (tokens->type == TOK_AND)
				op = tokens;
			else if (tokens->type == TOK_PIPE && !op)
				op = tokens;
		}
		// Passer au token suivant
		tokens = tokens->next;
	}
	return (op);
}

/**
 * @brief Coupe une liste de tokens en deux à partir d’un opérateur.
 *
 * @param tokens Début de la liste
 * @param split_token Le token à partir duquel on coupe
 * @return t_token* Pointeur vers le début de la partie droite
 */
static t_token *split_token_list(t_token *tokens, t_token *split_token)
{
	t_token *tmp;

	// ? Est-ce que c'est nécessaire de vérifier si les pointeurs sont valides ?
	if (!tokens || !split_token)
		return (NULL);
	// On copie le pointeur de tokens pour ne pas le modifier
	tmp = tokens;
	// Si le token à couper est le premier de la liste, on retourne le suivant
	// Cela permet de couper la liste à partir du token trouvé
	// et de retourner le reste de la liste
	if (tokens == split_token)
		return (split_token->next);
	// On avance dans la liste jusqu'à trouver le token juste avant split_token.
	while (tmp && tmp->next != split_token)
	tmp = tmp->next;
	// Une fois qu'on a trouvé ce token précédent, on coupe physiquement la liste, en mettant tmp->next à NULL.
	if (tmp)
		tmp->next = NULL;
	// On retourne le début de la partie droite, c’est-à-dire après l’opérateur.
	return (split_token->next);
}

/**
 * @brief Construit récursivement l’arbre logique d’exécution.
 *
 * @param tokens Liste de tokens
 * @return t_ast* Racine de l’AST
 */
t_ast *build_ast(t_token *tokens)
{
	t_token *op;
	t_ast *node;

	// Si la liste de tokens est vide, on retourne NULL
	if (!tokens)
		return (NULL);
	// Trouve le token opérateur principal
	op = find_main_operator(tokens);
	// Si aucun opérateur n'est trouvé, on crée un nœud de commande : AST_CMD
	if (!op)
		return (create_command_node(tokens));
	// On alloue de la mémoire pour un nouveau nœud AST
	node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	// On initialise le nœud en fonction du type de l'opérateur trouvé
	if (op->type == TOK_OR)
		node->type = AST_OR;
	else if (op->type == TOK_AND)
		node->type = AST_AND;
	else if (op->type == TOK_PIPE)
		node->type = AST_PIPE;
	// On initialise le nœud pour les commandes
	node->cmd = NULL;
	// Appel récursif pour construire la branche gauche
	node->left = build_ast(tokens);
	// Appel récursif pour construire la branche droite et on coupe la liste de tokens à partir de l'opérateur trouvé
	node->right = build_ast(split_token_list(tokens, op));
	return (node);
}
