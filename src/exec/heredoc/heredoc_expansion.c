/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expansion.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 09:57:18 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/04 17:57:07 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"

/**
 * @brief Expanse les variables dans une ligne de heredoc si nécessaire.
 *
 * Cette fonction applique l'expansion de variables uniquement si le délimiteur
 * original n'était pas quoté. Si il était quoté, la ligne est retournée telle quelle.
 *
 * @param line La ligne à traiter
 * @param expand_enabled 1 si l'expansion doit être appliquée, 0 sinon
 * @param shell Les données du shell pour l'expansion
 * @return La ligne traitée (expansée ou non selon le cas)
 */
char *expand_heredoc_line(char *line, int expand_enabled, t_shell *shell)
{
	char *expanded_line;

	if (!line)
		return (NULL);
	if (!expand_enabled)
		return (line);
	expanded_line = expand_variables(line, shell->env_list, shell->last_exit_status);
	free(line);
	return (expanded_line);
}

/**
 * @brief Nettoie le délimiteur en retirant toutes les quotes.
 *
 * Transforme :
 * - 'EOF' → EOF
 * - "EOF" → EOF
 * - ho"la" → hola
 * - h'ol'a → hola
 * - EOF   → EOF (inchangé)
 *
 * @param delimiter Le délimiteur original
 * @return Une copie allouée du délimiteur nettoyé
 */
char *clean_heredoc_delimiter(const char *delimiter)
{
	char *cleaned;
	int i, j;

	if (!delimiter)
		return (NULL);
	cleaned = malloc(ft_strlen(delimiter) + 1);
	if (!cleaned)
		return (NULL);
	i = 0;
	j = 0;
	while (delimiter[i])
	{
		if (delimiter[i] != '\'' && delimiter[i] != '"')
			cleaned[j++] = delimiter[i];
		i++;
	}
	cleaned[j] = '\0';
	return (cleaned);
}

/**
 * @brief Expanse les variables dans le délimiteur puis nettoie les guillemets.
 *
 * Cette fonction traite le délimiteur en deux étapes :
 * 1. Expansion des variables ($VAR, $"VAR", etc.)
 * 2. Nettoyage des guillemets
 *
 * Exemples :
 * - $"hola"$"b" → "" (si les variables n'existent pas)
 * - $HOME → /home/user (puis nettoie les guillemets s'il y en a)
 * - "EOF" → EOF
 *
 * @param delimiter Le délimiteur original
 * @param shell Les données du shell pour l'expansion
 * @return Le délimiteur expansé et nettoyé
 */
char *expand_and_clean_delimiter(const char *delimiter, t_shell *shell)
{
	char *expanded;
	char *cleaned;

	if (!delimiter)
		return (NULL);

	// Étape 1 : Expanser les variables dans le délimiteur
	expanded = expand_variables((char *)delimiter, shell->env_list, shell->last_exit_status);
	if (!expanded)
		return (NULL);

	// Étape 2 : Nettoyer les guillemets du délimiteur expansé
	cleaned = clean_heredoc_delimiter(expanded);
	free(expanded);

	return (cleaned);
}
