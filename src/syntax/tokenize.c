/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 17:22:42 by hugoganet         #+#    #+#             */
/*   Updated: 2025/05/29 08:57:04 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Crée un nouveau token avec sa valeur et son type.
 *
 * @param str Le texte du token
 * @param type  Le type (enum)
 * @return t_token* Le pointeur vers le nouveau token
 */
t_token *token_new(char *str, t_token_type type)
{
	t_token *token; // Pointeur vers le token

	token = malloc(sizeof(t_token)); // Allocation de mémoire pour le token
	if (!token)
		return (NULL);
	token->str = str; // Assigner la valeur du token
	token->type = type;	  // Assigner le type du token
	token->next = NULL;	  // Initialiser le pointeur suivant à NULL
	return (token);
}

/**
 * @brief Lit un token depuis l’entrée, ajoute son type et avance l’index.
 *
 * @param input La chaîne d’entrée
 * @param i Pointeur vers la position actuelle
 * @return t_token* Le token extrait, ou NULL si fin
 */
static t_token *get_next_token(char *input, int *i)
{
	int start;
	char *substr; // Sous-chaîne pour le token
	t_token_type type;

	while (input[*i] == ' ' || input[*i] == '\t') // Ignorer les espaces et tabulations
		(*i)++;
	if (input[*i] == '\0') // Si on atteint la fin de la chaîne
		return (NULL);
	start = *i; // Enregistrer le début du token
	// vérifier les opérateurs doubles et avancer l'index de 2
	if ((input[*i] == '<' && input[*i + 1] == '<') ||
		(input[*i] == '>' && input[*i + 1] == '>') ||
		(input[*i] == '&' && input[*i + 1] == '&') ||
		(input[*i] == '|' && input[*i + 1] == '|'))
		*i += 2;
	// vérifier les opérateurs simples et avancer l'index de 1
	else if (input[*i] == '<' || input[*i] == '>' || input[*i] == '|' || input[*i] == '(' || input[*i] == ')')
		(*i)++;
	else // Sinon, c'est un mot (commande ou argument)
	{
		// Avancer l'index jusqu'à la fin du mot
		while (input[*i] && input[*i] != ' ' && input[*i] != '\t' && input[*i] != '<' && input[*i] != '>' && input[*i] != '|' && input[*i] != '&' && input[*i] != '(' && input[*i] != ')')
			(*i)++;
	}
	if (start == *i) // Si on n'a pas avancé, c'est un token vide
		return (NULL);
	// Extraire le sous-texte du token
	substr = ft_substr(input, start, *i - start);
	if (!substr) // Vérifier l'allocation
		return (NULL);
	type = get_token_type(substr);
	return (token_new(substr, type));
}

/**
 * @brief Ajoute un token à la fin de la liste.
 *
 * @param head Début de la liste
 * @param last Dernier élément actuel
 * @param new Nouveau token à ajouter
 */
static void append_token(t_token **head, t_token **last, t_token *new)
{
	if (!*head)
		*head = new;
	else
		(*last)->next = new;
	*last = new;
}

/**
 * @brief Découpe la ligne en tokens simples (mots et symboles)
 *
 * @param input La ligne utilisateur
 * @return t_token* Liste chaînée de tokens
 */
t_token *tokenize(char *input)
{
	t_token *head = NULL;
	t_token *last = NULL;
	t_token *new;
	int i = 0;

	while (input[i])
	{
		new = get_next_token(input, &i);
		if (new)
			append_token(&head, &last, new);
	}
	refine_token_types(head);
	return (head);
}
