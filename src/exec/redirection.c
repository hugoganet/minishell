/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 15:30:08 by hugoganet         #+#    #+#             */
/*   Updated: 2025/06/13 16:46:04 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Extrait le nom du fichier à partir d'un token de redirection.
 *
 * Cette fonction saute les caractères d'opérateur ('<' ou '>') ainsi que les espaces
 * pour obtenir le nom du fichier qui suit l'opérateur dans une chaîne comme "<in.txt".
 *
 * @param str La chaîne contenant l'opérateur et le nom du fichier
 * @return Un pointeur vers le nom du fichier dans la chaîne d'origine
 */
static char *extract_filename(char *str)
{
	if (!str)
		return (NULL);
	// Avance au-delà des caractères '<' ou '>'
	while (*str == '<' || *str == '>')
		str++;
	// Ignore les espaces et tabulations éventuels après l'opérateur
	while (*str == ' ' || *str == '\t')
		str++;
	return (str);
}

/**
 * @brief Ouvre un fichier avec les bons flags selon le type de redirection.
 *
 * Gère l'ouverture en lecture seule pour les entrées, ou en écriture
 * avec création et/ou ajout ou écrasement pour les sorties.
 *
 * @param type Le type de redirection (REDIR_INPUT, REDIR_OUTPUT, REDIR_APPEND)
 * @param filename Le nom du fichier à ouvrir
 * @return Le descripteur de fichier ou -1 en cas d'échec
 */
static int open_redir_file(t_token_type type, char *filename)
{
	int fd;

	if (type == REDIR_INPUT)
		fd = open(filename, O_RDONLY);
	else if (type == REDIR_OUTPUT)
		fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (type == REDIR_APPEND)
		fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = -1;
	if (fd == -1)
		perror(filename);
	return (fd);
}

/**
 * @brief Applique récursivement les redirections définies dans l’AST.
 *
 * Cette fonction traverse une branche d’AST en partant d’un nœud redirection,
 * et applique chaque redirection trouvée à l'aide de `open()` et `dup2()`.
 * Si le type est REDIR_INPUT, redirige vers STDIN. Sinon, vers STDOUT.
 *
 * @param node Nœud courant (souvent le nœud racine passé à exec_cmd)
 * @return 0 si tout s'est bien passé, 1 en cas d'erreur
 */
int setup_redirections(t_ast *node)
{
	int		fd;
	char	*filename;

	// Vérifie que le nœud et sa chaîne sont valides
	if (!node || !node->str)
	{
		ft_putendl_fd("setup_redirections: invalid node or str", STDERR_FILENO);
		return (1);
	}
	// Parcours récursif à droite sur tous les nœuds de redirection
	while (node)
	{
		if (node->type == REDIR_INPUT || node->type == REDIR_OUTPUT || node->type == REDIR_APPEND)
		{
			// Extrait le nom du fichier cible
			filename = extract_filename(node->str);
			if (!filename || *filename == '\0')
			{
				ft_putendl_fd("setup_redirections: missing filename", STDERR_FILENO);
				return (1);
			}
			// Ouvre le fichier avec les bons flags
			fd = open_redir_file(node->type, filename);
			if (fd == -1)
				return (1);
			// Redirige STDIN ou STDOUT selon le type
			if ((node->type == REDIR_INPUT && dup2(fd, STDIN_FILENO) == -1) || (node->type != REDIR_INPUT && dup2(fd, STDOUT_FILENO) == -1))
			{
				perror("dup2");
				close(fd);
				return (1);
			}
			close(fd);
		}
		// Continue à parcourir les redirections à droite
		node = node->right;
	}
	return (0);
}