/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expansion.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 15:00:00 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/02 17:15:14 by hugoganet        ###   ########.fr       */
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
static int is_heredoc_delimiter_quoted(const char *delimiter)
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
	if (is_heredoc_delimiter_quoted(delimiter))
		return (ft_substr(delimiter, 1, len - 2));
	return (ft_strdup(delimiter));
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
static char *expand_heredoc_line(char *line, int expand_enabled, t_shell *shell)
{
	char *expanded_line;

	if (!line)
		return (NULL);
	if (!expand_enabled)
		return (line);
	expanded_line = join_str(ft_strdup(line), shell);
	free(line);
	return (expanded_line);
}

/**
 * @brief Initialise le pipe pour le heredoc.
 *
 * @param pipefd Tableau des descripteurs de pipe
 * @return 0 en cas de succès, 1 en cas d'erreur
 */
static int init_heredoc_pipe(int pipefd[2])
{
	if (pipe(pipefd) == -1)
	{
		perror("minishell: heredoc pipe");
		return (1);
	}
	return (0);
}

/**
 * @brief Valide le token du heredoc.
 *
 * @param token_str Le token complet du heredoc
 * @param pipefd Tableau des descripteurs de pipe (pour nettoyage si erreur)
 * @return 0 en cas de succès, 1 en cas d'erreur
 */
static int validate_heredoc_token(char *token_str, int pipefd[2])
{
	if (ft_strlen(token_str) <= 2)
	{
		perror("minishell: heredoc: missing delimiter");
		close(pipefd[0]);
		close(pipefd[1]);
		return (1);
	}
	return (0);
}

/**
 * @brief Vérifie si la ligne correspond au délimiteur.
 *
 * @param line La ligne lue
 * @param delimiter_clean Le délimiteur nettoyé
 * @return 1 si la ligne correspond au délimiteur, 0 sinon
 */
static int is_delimiter_line(char *line, char *delimiter_clean)
{
	int delim_len;

	if (!line || !delimiter_clean)
		return (0);
	delim_len = ft_strlen(delimiter_clean);
	if (ft_strncmp(line, delimiter_clean, delim_len) == 0 && line[delim_len] == '\n')
		return (1);
	return (0);
}

/**
 * @brief Traite une ligne du heredoc.
 *
 * @param line La ligne à traiter
 * @param delimiter_clean Le délimiteur nettoyé
 * @param expand_enabled 1 si l'expansion est activée, 0 sinon
 * @param shell Les données du shell
 * @param pipefd Descripteur de pipe en écriture
 * @return 1 pour continuer, 0 pour arrêter
 */
static int process_heredoc_input_line(char *line, char *delimiter_clean,
									  int expand_enabled, t_shell *shell, int pipefd)
{
	if (!line)
		return (0);
	if (is_delimiter_line(line, delimiter_clean))
	{
		free(line);
		return (0);
	}
	if (g_signal == SIGINT)
	{
		free(line);
		g_signal = 0;
		return (0);
	}
	line = expand_heredoc_line(line, expand_enabled, shell);
	if (line)
	{
		write(pipefd, line, ft_strlen(line));
		free(line);
	}
	return (1);
}

/**
 * @brief Lit et traite toutes les lignes du heredoc.
 *
 * @param delimiter_clean Le délimiteur nettoyé
 * @param expand_enabled 1 si l'expansion est activée, 0 sinon
 * @param shell Les données du shell
 * @param pipefd Descripteur de pipe en écriture
 */
static void read_heredoc_lines(char *delimiter_clean, int expand_enabled,
							   t_shell *shell, int pipefd)
{
	char *line;

	while (1)
	{
		line = get_next_line(STDIN_FILENO);
		if (!process_heredoc_input_line(line, delimiter_clean,
										expand_enabled, shell, pipefd))
			break;
	}
}

/**
 * @brief Finalise la redirection du heredoc.
 *
 * @param pipefd Tableau des descripteurs de pipe
 */
static void finalize_heredoc_redirection(int pipefd[2])
{
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
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
void handle_heredoc(char *token_str, t_shell *shell)
{
	char *delimiter_raw;
	char *delimiter_clean;
	int pipefd[2];
	int expand_enabled;

	if (init_heredoc_pipe(pipefd) != 0)
		return;
	if (validate_heredoc_token(token_str, pipefd) != 0)
		return;
	delimiter_raw = token_str + 2;
	expand_enabled = !is_heredoc_delimiter_quoted(delimiter_raw);
	delimiter_clean = clean_heredoc_delimiter(delimiter_raw);
	if (!delimiter_clean)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return;
	}
	read_heredoc_lines(delimiter_clean, expand_enabled, shell, pipefd[1]);
	free(delimiter_clean);
	finalize_heredoc_redirection(pipefd);
}
