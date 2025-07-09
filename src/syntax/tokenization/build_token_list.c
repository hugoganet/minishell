/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_token_list.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 16:57:59 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/09 10:10:02 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "syntax.h"

/**
 * @brief Crée un nouveau token avec sa valeur et son type.
 *
 * Alloue la mémoire pour un nouveau token et initialise ses champs.
 * Le pointeur 'next' est initialisé à NULL.
 *
 * @param str Le texte du token
 * @param type Le type (enum)
 * @return t_token* Le pointeur vers le nouveau token, ou NULL en cas d'erreur
 */
t_token	*token_new(char *str, t_token_type type)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->str = str;
	token->type = type;
	token->next = NULL;
	return (token);
}

/**
 * @brief Ajoute un token à la fin de la liste.
 *
 * Gère l'ajout d'un nouveau token à la fin d'une liste chaînée.
 * Met à jour les pointeurs de tête et de fin de liste.
 *
 * @param head Pointeur vers le début de la liste
 * @param last Pointeur vers le dernier élément actuel
 * @param new Nouveau token à ajouter
 */
void	append_token(t_token **head, t_token **last, t_token *new)
{
	if (!*head)
		*head = new;
	else
		(*last)->next = new;
	*last = new;
}
