/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 14:01:02 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/02 16:00:08 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Vérifie si une ligne est vide (ne contient que des espaces/tabs).
 *
 * @param input La ligne entrée par l’utilisateur.
 * @return int 1 si vide, 0 sinon.
 */
int is_line_empty(char *input)
{
	int i;

	if (!input)
		return (1);
	i = 0;
	// Parcourt la ligne jusqu'à la fin
	while (input[i])
	{
		// Vérifie si le caractère n'est pas un espace ou une tabulation
		// Si un caractère différent est trouvé, la ligne n'est pas vide, return 0
		if (input[i] != ' ' && input[i] != '\t')
			return (0);
		i++;
	}
	return (1);
}

/**
 * @brief Vérifie si l’entrée contient une quote non fermée.
 *
 * @param input La ligne entrée par l’utilisateur.
 * @return int 1 si une quote est non fermée, 0 sinon.
 */
int has_unclosed_quotes(char *input)
{
	int i;
	char quote_state;

	i = 0;
	// Initialise le caractère de quote actuel à 0 (aucune quote ouverte)
	quote_state = 0;
	while (input[i])
	{
		// Met à jour l'état de la quote
		update_quote_state(&quote_state, input[i]);
		i++;
	}
	// Forme simplifié de booléen.
	// Return 1 si current_quote n'est pas 0 (donc une quote est ouverte)
	// sinon return 0
	return (quote_state != 0);
}

/**
 * @brief Vérifie si les pipes sont mal placés : en début ou en fin.
 *
 * @param input Ligne entrée par l’utilisateur
 * @return int 1 si erreur, 0 si syntaxe correcte
 */
int has_invalid_pipes(char *input)
{
	int i;
	char quote_state;

	i = 0;
	quote_state = 0;
	while (input[i])
	{
		// Met à jour l'état de la quote
		update_quote_state(&quote_state, input[i]);
		// Si pas dans une quote et que le caractère est un pipe
		if (!quote_state && input[i] == '|')
		{
			// Si le pipe est au début ou à la fin de la ligne
			// Erreur de syntaxe, return 1
			if (i == 0 || input[i + 1] == '\0')
				return (1);
		}
		i++;
	}
	return (0);
}

/**
 * @brief Vérifie si les redirections sont mal placées ou incomplètes.
 *
 * @param input La ligne utilisateur
 * @return int 1 si erreur, 0 si syntaxe correcte
 */
int has_invalid_redirections(char *input)
{
	int i;
	char quote_state;
	int redir_len;

	i = 0;
	quote_state = 0;
	while (input[i])
	{
		// Met à jour l'état de la quote_state
		update_quote_state(&quote_state, input[i]);
		// Si pas dans une quote et que le caractère est une redirection
		if (!quote_state && (input[i] == '<' || input[i] == '>'))
		{
			// Par défaut : redirection simple
			redir_len = 1;
			// Si redirection double (ex: >> ou <<)
			// On augmente la longueur de la redirection
			if (input[i + 1] == input[i])
				redir_len = 2;
			// On saute les caractères de redirection
			i += redir_len;
			// Ignore les espaces et tabulations
			while (input[i] == ' ' || input[i] == '\t')
				i++;
			// Si fin de ligne ou opérateur juste après
			// Erreur de syntaxe, return 1
			if (input[i] == '\0' || input[i] == '|' || input[i] == '<' || input[i] == '>')
				return (1);
			// On évite le i++ final pour ne pas sauter un caractère utile
			continue;
		}
		i++;
	}
	return (0);
}

/**
 * @brief Vérifie que les parenthèses sont équilibrées et non vides.
 *
 * @param input Ligne de commande
 * @return int 1 si erreur, 0 si OK
 */
int has_unmatched_parentheses(char *input)
{
	int i;
	int count;
	char quote;

	i = 0;
	count = 0;
	quote = 0;
	while (input[i])
	{
		// Met à jour l'état de la quote
		update_quote_state(&quote, input[i]);
		// Si pas dans une quote et que le caractère est une parenthèse ouvrante
		if (!quote && input[i] == '(')
		{
			// On incrémente le compteur de parenthèses
			count++;
			// Vérifie si la parenthèse est vide
			// Erreur : parenthèse vide
			if (is_parenthesis_empty(input, i))
				return (1);
		}
		// Si pas dans une quote et que le caractère est une parenthèse fermante
		else if (!quote && input[i] == ')')
		{
			// Si le compteur est à 0, il y a une parenthèse fermante sans ouvrante
			// Erreur : parenthèse fermante sans ouvrante, return 1
			if (count == 0)
				return (1);
			// On décrémente le compteur de parenthèses
			count--;
		}
		i++;
	}
	return (count != 0);
}

/**
 * @brief Vérifie que les accolades sont équilibrées.
 *
 * @param input Ligne de commande
 * @return `int` 1 si erreur (accolades non fermées), 0 si OK
 */
int has_unclosed_braces(char *input)
{
	int i;
	int count;
	char quote_state;

	i = 0;
	count = 0;
	quote_state = 0;
	while (input[i])
	{
		// Met à jour l'état de la quote
		update_quote_state(&quote_state, input[i]);
		// Si pas dans une quote et que le caractère est une accolade ouvrante
		if (!quote_state && input[i] == '{')
			count++;
		// Si pas dans une quote et que le caractère est une accolade fermante
		else if (!quote_state && input[i] == '}')
		{
			// Si le compteur est à 0, il y a une accolade fermante sans ouvrante
			// Erreur : accolade fermante sans ouvrante, return 1
			if (count == 0)
				return (1);
			// On décrémente le compteur d'accolades
			count--;
		}
		i++;
	}
	// Return 1 si count != 0 (accolades non fermées), 0 sinon
	return (count != 0);
}
