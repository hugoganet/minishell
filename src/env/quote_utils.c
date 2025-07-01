/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 12:00:00 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/01 11:51:44 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Met à jour l'état des quotes selon le caractère courant.
 *
 * Cette fonction gère la logique de basculement des états de quotes simples
 * et doubles. Une quote simple ne peut être ouverte que si on n'est pas dans
 * une quote double, et vice versa.
 *
 * @param c Le caractère courant à analyser
 * @param in_single Pointeur vers l'état des quotes simples
 * @param in_double Pointeur vers l'état des quotes doubles
 */
static void update_quote_status(char c, bool *in_single, bool *in_double)
{
	if (c == '\'' && !(*in_double))
		*in_single = !(*in_single);
	else if (c == '"' && !(*in_single))
		*in_double = !(*in_double);
}

/**
 * @brief Détermine si le `$` à la position donnée doit être expansé.
 *
 * L'expansion des variables est interdite à l'intérieur de quotes simples,
 * mais autorisée dans les quotes doubles et hors quotes.
 * Cette fonction parcourt la chaîne jusqu'à la position donnée pour
 * déterminer l'état des quotes à cette position.
 *
 * @param str La chaîne d'origine à analyser
 * @param pos L'index du `$` dans la chaîne
 * @return true si l'expansion est autorisée, false sinon
 */
bool should_expand_at_position(const char *str, int pos)
{
	bool in_single;
	bool in_double;
	int i;

	in_single = false;
	in_double = false;
	i = 0;
	// Parcours de la chaîne jusqu'à la position du `$`
	while (i < pos && str[i])
	{
		update_quote_status(str[i], &in_single, &in_double);
		i++;
	}
	// Dans les quotes simples, pas d'expansion
	return (!in_single);
}

/**
 * @brief Vérifie si une variable dans la chaîne doit être expansée.
 *
 * Cette fonction parcourt toute la chaîne à la recherche d'un `$` qui
 * pourrait être expansé selon les règles des quotes.
 * - Entre quotes simples : pas d'expansion
 * - Entre quotes doubles ou hors quotes : expansion autorisée
 *
 * @param str La chaîne à vérifier
 * @return true si au moins une variable peut être expansée, false sinon
 */
bool is_expandable(const char *str)
{
	bool in_single;
	bool in_double;
	int i;

	in_single = false;
	in_double = false;
	i = 0;
	while (str[i])
	{
		update_quote_status(str[i], &in_single, &in_double);
		// Variable à expanser trouvée ($ hors quotes simples)
		if (str[i] == '$' && !in_single)
			return (true);
		i++;
	}
	// Aucune variable à expanser trouvée
	return (false);
}

/**
 * @brief Détermine le type de quote à une position donnée.
 *
 * Cette fonction analyse la chaîne jusqu'à la position donnée pour
 * déterminer si on se trouve dans des quotes simples, doubles, ou aucune.
 *
 * @param str La chaîne à analyser
 * @param pos La position à vérifier
 * @param quote_type Pointeur pour stocker le type de quote (' ou " ou 0)
 * @return true si on est dans des quotes, false sinon
 */
bool is_in_quotes(const char *str, int pos, char *quote_type)
{
	bool in_single;
	bool in_double;
	int i;

	in_single = false;
	in_double = false;
	i = 0;
	while (i < pos && str[i])
	{
		update_quote_status(str[i], &in_single, &in_double);
		i++;
	}
	if (in_single)
	{
		*quote_type = '\'';
		return (true);
	}
	if (in_double)
	{
		*quote_type = '"';
		return (true);
	}
	*quote_type = 0;
	return (false);
}
