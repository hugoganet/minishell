/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expansion_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 09:57:18 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/03 11:14:54 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Détermine si le délimiteur heredoc est quoté.
 *
 * Analyse le délimiteur pour détecter :
 * - <<EOF    → pas de quotes (expansion activée)
 * - <<'EOF'  → quotes simples (expansion désactivée)
 * - <<"EOF"  → quotes doubles (expansion désactivée)
 *
 * @param delimiter Le délimiteur original (ex: "EOF", "'EOF'", "\"EOF\"")
 * @return 1 si le délimiteur est quoté (expansion désactivée), 0 sinon
 */
int is_heredoc_delimiter_quoted(const char *delimiter)
{
	int len;

	if (!delimiter)
		return (0);
	len = ft_strlen(delimiter);
	if (len < 2)
		return (0);
	if (delimiter[0] == '\'' && delimiter[len - 1] == '\'')
		return (1);
	if (delimiter[0] == '"' && delimiter[len - 1] == '"')
		return (1);
	return (0);
}

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