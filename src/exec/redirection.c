/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 15:30:08 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/02 17:06:18 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Applique les redirections dans le processus parent.
 *
 * Utilisée pour les redirections seules comme '<input.txt'
 * qui ne nécessitent pas d'exécuter une commande enfant.
 *
 * @param node Le nœud de redirection à traiter
 * @param shell Structure contenant les données du shell
 * @return Code de retour (0 pour succès, autre pour erreur)
 */
int apply_parent_redirections(t_ast *node, t_shell *shell)
{
	int saved_stdin;
	int saved_stdout;
	int result;

	// Sauvegarde des descripteurs de fichiers originaux
	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	result = 0;
	if (node->type == HEREDOC)
		handle_heredoc(node->str, shell);
	if (setup_redirections(node) != 0)
		result = 1;
	// Restaure les descripteurs originaux
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
	return (result);
}

/**
 * @brief Extrait le nom du fichier à partir d'un token de redirection.
 *
 * Saute les caractères d'opérateur ('<' ou '>') et les espaces
 * pour obtenir le nom du fichier.
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
	while (*str == ' ' || *str == '\t')
		str++;
	return (str);
}

/**
 * @brief Ouvre un fichier avec les bons flags selon le type de redirection.
 *
 * Gère l'ouverture en lecture pour les entrées, ou en écriture
 * avec création et/ou ajout ou écrasement pour les sorties.
 *
 * @param type Le type de redirection
 * @param filename Le nom du fichier à ouvrir
 * @return Le descripteur de fichier ou -1 en cas d'échec
 */
static int open_redir_file(t_token_type type, char *filename)
{
	int fd;

	fd = -1;
	if (type == REDIR_INPUT)
		fd = open(filename, O_RDONLY);
	else if (type == REDIR_OUTPUT)
		fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (type == REDIR_APPEND)
		fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
		perror(filename);
	return (fd);
}

/**
 * @brief Applique une redirection spécifique (dup2 vers STDIN ou STDOUT).
 *
 * @param type Le type de redirection
 * @param fd Le descripteur de fichier source
 * @return 0 en cas de succès, 1 en cas d'erreur
 */
static int apply_single_redirection(t_token_type type, int fd)
{
	int result;

	result = 0;
	if (type == REDIR_INPUT)
		result = dup2(fd, STDIN_FILENO);
	else
		result = dup2(fd, STDOUT_FILENO);
	if (result == -1)
	{
		perror("dup2");
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

/**
 * @brief Traite un nœud de redirection unique.
 *
 * @param node Le nœud de redirection à traiter
 * @return 0 en cas de succès, 1 en cas d'erreur
 */
static int process_redirection_node(t_ast *node)
{
	char *filename;
	int fd;

	if (node->type != REDIR_INPUT && node->type != REDIR_OUTPUT && node->type != REDIR_APPEND)
		return (0);
	filename = extract_filename(node->str);
	if (!filename || *filename == '\0')
	{
		ft_putendl_fd("setup_redirections: missing filename", STDERR_FILENO);
		return (1);
	}
	fd = open_redir_file(node->type, filename);
	if (fd == -1)
		return (1);
	return (apply_single_redirection(node->type, fd));
}

/**
 * @brief Applique récursivement les redirections définies dans l'AST.
 *
 * Traverse une branche d'AST en partant d'un nœud redirection,
 * et applique chaque redirection trouvée.
 *
 * @param node Nœud courant
 * @return 0 si tout s'est bien passé, 1 en cas d'erreur
 */
int setup_redirections(t_ast *node)
{
	if (!node || !node->str)
	{
		ft_putendl_fd("setup_redirections: invalid node or str",
					  STDERR_FILENO);
		return (1);
	}
	while (node)
	{
		if (process_redirection_node(node) != 0)
			return (1);
		node = node->right;
	}
	return (0);
}
