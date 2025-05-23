/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 14:01:02 by hugoganet         #+#    #+#             */
/*   Updated: 2025/05/23 16:06:46 by hugoganet        ###   ########.fr       */
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
	while (input[i]) // Parcourt la ligne jusqu'à la fin
	{
		if (input[i] != ' ' && input[i] != '\t') // Vérifie si le caractère n'est pas un espace ou une tabulation
			return (0); // Si un caractère différent est trouvé, la ligne n'est pas vide, return 0
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
	quote_state = 0; // Initialise le caractère de quote actuel à 0 (aucune quote ouverte)
	while (input[i])
	{
		update_quote_state(&quote_state, input[i]); // Met à jour l'état de la quote
		i++;
	}
	return (quote_state != 0); // Forme simplifié de booléen. Return 1 si current_quote n'est pas 0 (donc une quote est ouverte), sinon return 0
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
		update_quote_state(&quote_state, input[i]); // Met à jour l'état de la quote
		if (!quote_state && input[i] == '|') // Si pas dans une quote et que le caractère est un pipe
		{
			if (i == 0 || input[i + 1] == '\0') // Si le pipe est au début ou à la fin de la ligne
				return (1); // Erreur de syntaxe
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
		update_quote_state(&quote_state, input[i]); // Met à jour l'état de la quote_state
		if (!quote_state && (input[i] == '<' || input[i] == '>')) // Si pas dans une quote_state et que le caractère est une redirection			
		{
			redir_len = 1; // Par défaut : redirection simple
			if (input[i + 1] == input[i]) // Si redirection double (ex: >> ou <<)
				redir_len = 2; // On augmente la longueur de la redirection
			i += redir_len;	 // On saute les caractères de redirection
			while (input[i] == ' ' || input[i] == '\t') // Ignore les espaces et tabulations
				i++;
			if (input[i] == '\0' || input[i] == '|' || input[i] == '<' || input[i] == '>') // Erreur : fin de ligne ou opérateur juste après
				return (1);
			continue; // On évite le i++ final pour ne pas sauter un caractère utile
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
	int j;
	int count;
	char quote;

	i = 0;
	count = 0;
	quote = 0;
	while (input[i])
	{
		update_quote_state(&quote, input[i]); // Met à jour l'état de la quote
		if (!quote && input[i] == '(') // Si pas dans une quote et que le caractère est une parenthèse ouvrante
		{
			count++; // Incrémente le compteur de parenthèses ouvrantes
			j = i + 1; // Set le j à la position suivante
			while (input[j] == ' ' || input[j] == '\t') // Ignore les espaces et tabulations
				j++;
			if (input[j] == ')') // Si la parenthèse ouvrante est suivie d'une parenthèse fermante
				return (1); // Erreur : parenthèse vide
		}
		else if (!quote && input[i] == ')') // Si pas dans une quote et que le caractère est une parenthèse fermante
		{
			if (count == 0) // Si le compteur est à 0, il n'y a pas de parenthèse ouvrante correspondante
				return (1); // Erreur : parenthèse fermante sans ouvrante
			count--; // Décrémente le compteur de parenthèses ouvrantes
		}
		i++;
	}
	return (count != 0); // Si > 0, il manque des fermetures
}
