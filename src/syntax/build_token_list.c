/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_token_list.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 16:57:59 by hugoganet         #+#    #+#             */
/*   Updated: 2025/05/29 16:58:01 by hugoganet        ###   ########.fr       */
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
	token->str = str;	// Assigner la valeur du token
	token->type = type; // Assigner le type du token
	token->next = NULL; // Initialiser le pointeur suivant à NULL
	return (token);
}

/**
 * @brief Ajoute un token à la fin de la liste.
 *
 * @param head Début de la liste
 * @param last Dernier élément actuel
 * @param new Nouveau token à ajouter
 */
void append_token(t_token **head, t_token **last, t_token *new)
{
	if (!*head)
		*head = new;
	else
		(*last)->next = new;
	*last = new;
}