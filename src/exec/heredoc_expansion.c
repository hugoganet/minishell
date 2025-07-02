/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expansion.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 15:00:00 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/02 16:46:53 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Détermine si le délimiteur heredoc est quoté et quel type de quote.
 *
 * Analyse le délimiteur pour détecter :
 * - <<EOF    → pas de quotes (expansion activée)
 * - <<'EOF'  → quotes simples (expansion désactivée)
 * - <<"EOF"  → quotes doubles (expansion désactivée)
 *
 * @param delimiter Le délimiteur original (ex: "EOF", "'EOF'", "\"EOF\"")
 * @return true si le délimiteur est quoté (expansion désactivée), false sinon
 */
static bool is_heredoc_delimiter_quoted(const char *delimiter)
{
	int len;

	if (!delimiter)
		return (false);
	len = ft_strlen(delimiter);
	if (len < 2)
		return (false);
	// Vérifie les quotes simples : 'EOF'
	if (delimiter[0] == '\'' && delimiter[len - 1] == '\'')
		return (true);
	// Vérifie les quotes doubles : "EOF"
	if (delimiter[0] == '"' && delimiter[len - 1] == '"')
		return (true);
	return (false);
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
static char *clean_heredoc_delimiter(const char *delimiter)
{
	int len;

	if (!delimiter)
		return (NULL);
	len = ft_strlen(delimiter);
	// Si le délimiteur est quoté, retirer les quotes
	if (is_heredoc_delimiter_quoted(delimiter))
		return (ft_substr(delimiter, 1, len - 2));
	// Sinon, retourner une copie du délimiteur original
	return (ft_strdup(delimiter));
}

/**
 * @brief Expanse les variables dans une ligne de heredoc si nécessaire.
 *
 * Cette fonction applique l'expansion de variables uniquement si le délimiteur
 * original n'était pas quoté. Si il était quoté, la ligne est retournée telle quelle.
 *
 * @param line La ligne à traiter
 * @param expand_enabled true si l'expansion doit être appliquée
 * @param shell Les données du shell pour l'expansion
 * @return La ligne traitée (expansée ou non selon le cas)
 */
static char *expand_heredoc_line(char *line, bool expand_enabled, t_shell *shell)
{
	char *expanded_line;

	if (!line)
		return (NULL);
	// Si l'expansion est désactivée, retourner la ligne originale
	if (!expand_enabled)
		return (line);
	// Appliquer l'expansion de variables
	expanded_line = join_str(ft_strdup(line), shell);
	free(line);
	return (expanded_line);
}

/**
 * @brief Gère un heredoc avec expansion conditionnelle des variables.
 *
 * Cette fonction implémente le comportement complet des heredocs de Bash :
 * 1. Analyse le délimiteur pour détecter les quotes
 * 2. Détermine si l'expansion doit être activée ou non
 * 3. Lit les lignes et applique l'expansion conditionnellement
 * 4. Écrit le résultat dans un pipe et redirige STDIN
 *
 * @param token_str Le token complet du heredoc (ex: "<<EOF", "<<'END'")
 * @param shell Les données du shell pour l'expansion de variables
 */
void handle_heredoc_with_expansion(char *token_str, t_shell *shell)
{
	char *delimiter_raw;
	char *delimiter_clean;
	char *line;
	int pipefd[2];
	bool expand_enabled;

	if (pipe(pipefd) == -1)
	{
		perror("minishell: heredoc pipe");
		return;
	}
	// Vérifie que le token_str est valide (doit commencer par "<<")
	if (ft_strlen(token_str) <= 2)
	{
		perror("minishell: heredoc: missing delimiter");
		close(pipefd[0]);
		close(pipefd[1]);
		return;
	}
	// Extrait le délimiteur (après "<<")
	delimiter_raw = token_str + 2;
	// Détermine si l'expansion est activée (délimiteur non quoté)
	expand_enabled = !is_heredoc_delimiter_quoted(delimiter_raw);
	// Nettoie le délimiteur (retire les quotes si présentes)
	delimiter_clean = clean_heredoc_delimiter(delimiter_raw);
	if (!delimiter_clean)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return;
	}
	// Boucle pour lire les lignes jusqu'au délimiteur
	while (1)
	{
		line = get_next_line(STDIN_FILENO);
		if (!line)
			break;
		// Vérifie si la ligne correspond au délimiteur propre
		if (ft_strncmp(line, delimiter_clean, ft_strlen(delimiter_clean)) == 0 && line[ft_strlen(delimiter_clean)] == '\n')
		{
			free(line);
			break;
		}
		// Vérifie si SIGINT (Ctrl+C) a été reçu
		if (g_signal == SIGINT)
		{
			free(line);
			g_signal = 0;
			break;
		}
		// Expanse la ligne si nécessaire
		line = expand_heredoc_line(line, expand_enabled, shell);
		// Écrit la ligne (expansée ou non) dans le pipe
		if (line)
		{
			write(pipefd[1], line, ft_strlen(line));
			free(line);
		}
	}
	// Nettoyage et redirection
	free(delimiter_clean);
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
}
