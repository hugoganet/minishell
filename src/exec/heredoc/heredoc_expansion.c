/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expansion.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 09:57:18 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/04 09:06:22 by hugoganet        ###   ########.fr       */
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
 * @brief Nettoie le délimiteur en retirant les quotes si présentes.
 *
 * Transforme :
 * - 'EOF' → EOF
 * - "EOF" → EOF
 * - EOF   → EOF (inchangé)
 *
 * @param delimiter Le délimiteur original
 * @return Une copie allouée du délimiteur nettoyé
 */
char *clean_heredoc_delimiter(const char *delimiter)
{
	int len;

	if (!delimiter)
		return (NULL);
	len = ft_strlen(delimiter);
	if (is_heredoc_delimiter_quoted(delimiter))
		return (ft_substr(delimiter, 1, len - 2));
	return (ft_strdup(delimiter));
}
