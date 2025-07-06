/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_core.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 00:00:00 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/06 20:02:03 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "syntax.h"

/**
 * @brief Lit un token complet (quote, mot ou opérateur) à partir de l'index donné
 *
 * Crée systématiquement un nouveau token dès qu'un bloc (mot, quote ou opérateur) est détecté.
 * Gère les différents types de tokens : redirections combinées, mots complets, opérateurs.
 *
 * @param input Ligne utilisateur
 * @param i Pointeur vers l'index actuel (sera mis à jour)
 * @return t_token* Un token alloué, ou NULL en cas d'erreur
 */
static t_token *get_next_token(char *input, int *i)
{
	char *content;
	t_token_type type;

	if ((input[*i] == '<' || input[*i] == '>') && input[*i] != '\0')
		content = read_redir_and_file(input, i);
	else if (!is_token_delim(input[*i]) && input[*i])
		content = read_complete_token(input, i);
	else
		content = read_operator(input, i);
	if (!content)
		return (NULL);
	type = get_token_type(content);
	return (token_new(content, type));
}

/**
 * @brief Traite un token et l'ajoute à la liste si valide
 *
 * Cette fonction gère l'ajout d'un nouveau token à la liste chaînée
 * avec vérification d'erreur et nettoyage en cas d'échec.
 *
 * @param input La chaîne d'entrée (pour les messages d'erreur)
 * @param i Pointeur vers l'index courant
 * @param head Pointeur vers la tête de liste
 * @param last Pointeur vers le dernier élément
 * @return int 0 en cas de succès, -1 en cas d'erreur
 */
static int process_and_add_token(char *input, int *i, t_token **head,
								 t_token **last)
{
	t_token *new;

	new = get_next_token(input, i);
	if (!new && input[*i])
	{
		ft_putendl_fd("minishell: error: failed to tokenize input", 2);
		free_token_list(*head);
		return (-1);
	}
	if (new)
		append_token(head, last, new);
	return (0);
}

/**
 * @brief Effectue le post-traitement des tokens
 *
 * Applique les transformations finales sur la liste de tokens :
 * - Affinement des types (CMD, ARG, etc.)
 * - Validation syntaxique
 *
 * @param head Pointeur vers la tête de liste
 * @return int 0 en cas de succès, -1 en cas d'erreur syntaxique
 */
static int finalize_tokens(t_token **head)
{
	refine_token_types(*head);
	if (validate_token_sequence(*head))
	{
		free_token_list(*head);
		return (-1);
	}
	return (0);
}

/**
 * @brief Fonction principale de découpage : transforme la ligne en une liste de tokens
 *
 * Parcourt la chaîne d'entrée caractère par caractère, identifie les tokens
 * et les ajoute à une liste chaînée. Effectue ensuite le post-traitement
 * pour affiner les types et valider la syntaxe.
 *
 * @param input Chaîne entrée par l'utilisateur
 * @return t_token* Liste chaînée des tokens, ou NULL en cas d'erreur
 */
t_token *tokenize(char *input)
{
	t_token *head;
	t_token *last;
	int i;

	head = NULL;
	last = NULL;
	i = 0;
	skip_spaces(input, &i);
	while (input[i])
	{
		if (process_and_add_token(input, &i, &head, &last) == -1)
			return (NULL);
		skip_spaces(input, &i);
	}
	if (finalize_tokens(&head) == -1)
		return (NULL);
	return (head);
}
