/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 09:49:38 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/01 10:09:23 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Met à jour l’état des quotes selon le caractère courant.
 *
 * @param c Caractère courant.
 * @param in_single Pointeur vers état quote simple.
 * @param in_double Pointeur vers état quote double.
 */
static void update_quote_status(char c, bool *in_single, bool *in_double)
{
	if (c == '\'' && !(*in_double))
		*in_single = !(*in_single);
	else if (c == '"' && !(*in_single))
		*in_double = !(*in_double);
}

/**
 * @brief Copie la chaîne en retirant les caractères de quote, mais pas leur contenu.
 *
 * @param dst Destination.
 * @param src Source.
 */
static void copy_without_quotes(char *dst, const char *src)
{
	int i;
	int j;
	bool in_single;
	bool in_double;

	i = 0;
	j = 0;
	in_single = false;
	in_double = false;
	while (src[i])
	{
		if ((src[i] == '\'' && !in_double) || (src[i] == '"' && !in_single))
			update_quote_status(src[i], &in_single, &in_double);
		else
			dst[j++] = src[i];
		i++;
	}
}

/**
 * @brief Supprime les quotes simples et doubles d’une chaîne selon le contexte.
 *
 * Cette fonction conserve le contenu entre les quotes, mais supprime les caractères
 * de quote eux-mêmes. Les quotes simples bloquent toute expansion, tandis que
 * les quotes doubles autorisent l’expansion du `$`.
 *
 * @param str Chaîne originale.
 * @return Chaîne nettoyée (à free), ou NULL si erreur mémoire.
 */
char *remove_quotes(char *str)
{
	char *result;

	if (!str)
		return (NULL);
	result = ft_calloc(ft_strlen(str) + 1, sizeof(char));
	if (!result)
		return (NULL);
	copy_without_quotes(result, str);
	return (result);
}