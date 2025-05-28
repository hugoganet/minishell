/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 17:26:12 by hugoganet         #+#    #+#             */
/*   Updated: 2025/05/28 17:40:01 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Associe une chaîne à son type de token.
 *
 * @param str Token brut
 * @return t_token_type Type du token
 */
t_token_type get_token_type(char *str)
{
	if (!str)
		return (WORD);
	if (!ft_strncmp(str, "&&", 3))
		return (AND);
	if (!ft_strncmp(str, "||", 3))
		return (OR);
	if (!ft_strncmp(str, "<<", 3))
		return (HEREDOC);
	if (!ft_strncmp(str, ">>", 3))
		return (REDIR_APPEND);
	if (!ft_strncmp(str, "<", 2))
		return (REDIR_INPUT);
	if (!ft_strncmp(str, ">", 2))
		return (REDIR_OUTPUT);
	if (!ft_strncmp(str, "|", 2))
		return (PIPE);
	if (!ft_strncmp(str, "(", 2))
		return (PAREN_LEFT);
	if (!ft_strncmp(str, ")", 2))
		return (PAREN_RIGHT);
	return (WORD);
}

/**
 * @brief Vérifie si le type de token correspond à une redirection.
 *
 * @param type Le type du token à vérifier
 * @return true si c'est une redirection (<, >, <<, >>), false sinon
 */
bool is_redirection(t_token_type type)
{
	return (type == REDIR_INPUT || type == REDIR_OUTPUT || type == REDIR_APPEND || type == HEREDOC);
}

/**
 * @brief Reclasse les tokens WORD en CMD, ARG ou FILES selon leur contexte.
 *
 * Cette fonction est appelée après la tokenisation brute. Elle examine la liste
 * chaînée de tokens et met à jour le type des tokens initialement de type WORD
 * en fonction de leur rôle dans la commande :
 *
 * - Le premier WORD est une CMD
 * - Un WORD après une redirection devient FILES
 * - Un WORD après une CMD devient ARG
 * - Un WORD après un PIPE devient CMD
 *
 * @param head Pointeur vers le premier token de la liste
 */
void logic_groups(t_token *head)
{
	t_token *ptr; // ? renomer ptr en curr

	ptr = head;
	// Le tout premier WORD est une commande
	if (ptr && ptr->type == WORD)
	{
		ptr->type = CMD;
		printf("Token %s is %u\n", ptr->value, ptr->type);
	}
	while (ptr)
	{
		// Si le token courant est une redirection et qu'il y a un token suivant,
		// on le transforme en FILES
		if (is_redirection(ptr->type) && ptr->next)
			ptr->next->type = FILES;
		// Si le token courant est une CMD et qu'il y a un token suivant de type WORD,
		// on le transforme en ARG
		else if (ptr->type == CMD && ptr->next && ptr->next->type == WORD)
			ptr->next->type = ARG;
		// Si le token courant est un PIPE et qu'il y a un token suivant de type WORD,
		// on le transforme en CMD
		else if (ptr->type == PIPE && ptr->next && ptr->next->type == WORD)
			ptr->next->type = CMD;
		ptr = ptr->next;
	}
}