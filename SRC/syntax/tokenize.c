/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 17:22:42 by hugoganet         #+#    #+#             */
/*   Updated: 2025/05/23 17:25:22 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Crée un nouveau token avec sa valeur et son type.
 *
 * @param value Le texte du token
 * @param type  Le type (enum)
 * @return t_token* Le pointeur vers le nouveau token
 */
t_token *token_new(char *value, t_token_type type)
{
	t_token *token; // Pointeur vers le token

	token = malloc(sizeof(t_token)); // Allocation de mémoire pour le token
	if (!token)
		return (NULL);
	token->value = value; // Assigner la valeur du token
	token->type = type; // Assigner le type du token
	token->next = NULL; // Initialiser le pointeur suivant à NULL
	return (token);
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
	int i = 0;

	while (input[i])
	{
		while (input[i] == ' ' || input[i] == '\t')
			i++;
		if (input[i] == '\0')
			break;
		int start = i;
		if ((input[i] == '<' || input[i] == '>') && input[i + 1] == input[i])
			i += 2;
		else if (input[i] == '<' || input[i] == '>' || input[i] == '|' || input[i] == '(' || input[i] == ')')
			i++;
		else
		{
			// mot (commande ou argument)
			while (input[i] && input[i] != ' ' && input[i] != '\t' &&
				   input[i] != '<' && input[i] != '>' && input[i] != '|' && input[i] != '(' && input[i] != ')')
				i++;
		}

		char *substr = ft_substr(input, start, i - start);
		t_token_type type = get_token_type(substr);
		t_token *new = token_new(substr, type);

		if (!head)
			head = new;
		else
			last->next = new;
		last = new;
	}
	return (head);
}
