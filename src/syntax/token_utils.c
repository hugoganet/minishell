/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 17:26:12 by hugoganet         #+#    #+#             */
/*   Updated: 2025/05/30 10:07:11 by hugoganet        ###   ########.fr       */
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
void refine_token_types(t_token *head)
{
	t_token *curr;

	curr = head;
	// Le tout premier WORD est une commande
	if (curr && curr->type == WORD)
		curr->type = CMD;
	while (curr)
	{
		// Si le token courant est une redirection et qu'il y a un token suivant,
		// on le transforme en FILES
		if (is_redirection(curr->type) && curr->next)
			curr->next->type = FILES;
		// Si le token courant est une CMD et qu'il y a un token suivant de type WORD,
		// on le transforme en ARG
		else if (curr->type == CMD && curr->next && curr->next->type == WORD)
			curr->next->type = ARG;
		// Si le token courant est un PIPE et qu'il y a un token suivant de type WORD,
		// on le transforme en CMD
		else if (curr->type == PIPE && curr->next && curr->next->type == WORD)
			curr->next->type = CMD;
		curr = curr->next;
	}
}

/**
 * @brief Extrait une sous-chaîne entre quotes simples ou doubles.
 *
 * @param input Chaîne d'entrée
 * @param i Pointeur vers l'index courant (pointant sur la quote ouvrante)
 * @return char* Sous-chaîne allouée (sans les quotes), ou NULL en cas d'erreur
 */
char *parse_quoted_token(char *input, int *i)
{
	char quote;
	int start;
	int end;
	char *token_new;

	quote = input[*i];
	start = *i;
	// On commence à parcourir input juste après la quote ouvrante
	end = start + 1;
	// Avance l'index jusqu'à la quote fermante correspondante	
	while (input[end] && input[end] != quote)
		end++;
	// On set l'index à la fin de la quote fermante
	*i = end + 1;
	// On extrait la sous-chaîne avec les quotes
	token_new = ft_substr(input, start + 1, end - start - 1);
	if (!token_new)
		return (NULL);
	return (token_new); 
}

/**
 * @brief Vérifie la validité syntaxique de la séquence de tokens.
 *
 * Cette fonction s'assure qu'il n'y a pas d'opérateurs consécutifs,
 * ni d'opérateur en début ou fin de ligne, ni d'opérateurs sans commande valide.
 * Affiche un message d'erreur approprié en cas d'erreur.
 *
 * @param head Pointeur vers le premier token de la liste
 * @return int 0 si la séquence est valide, 1 sinon
 */
int validate_token_sequence(t_token *head)
{
    t_token *curr;
    t_token *prev;

    if (!head)
        return (0);
    curr = head;
    prev = NULL;
    while (curr)
    {
        // Vérifie opérateur en début de ligne
        if (!prev && (curr->type == PIPE || curr->type == OR || curr->type == AND))
        {
            ft_putendl_fd("minishell: syntax error near unexpected token '" \
                "'", 2);
            return (1);
        }
        // Vérifie deux opérateurs consécutifs
        if (prev &&
            (prev->type == PIPE || prev->type == OR || prev->type == AND) &&
            (curr->type == PIPE || curr->type == OR || curr->type == AND))
        {
            ft_putstr_fd("minishell: syntax error near unexpected token '", 2);
            ft_putstr_fd(curr->str, 2);
            ft_putendl_fd("'", 2);
            return (1);
        }
        prev = curr;
        curr = curr->next;
    }
    // Vérifie opérateur en fin de ligne
    if (prev && (prev->type == PIPE || prev->type == OR || prev->type == AND))
    {
        ft_putstr_fd("minishell: syntax error near unexpected token '", 2);
        ft_putstr_fd(prev->str, 2);
        ft_putendl_fd("'", 2);
        return (1);
    }
    return (0);
}
