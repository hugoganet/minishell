/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 00:00:00 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/09 17:03:48 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Extrait le nom de fichier d'une chaîne de redirection.
 *
 * Ignore les chevrons ('<' ou '>') et les espaces/tabs au début de la chaîne,
 * pour retourner le nom du fichier cible de la redirection.
 *
 * @param str La chaîne de redirection à traiter (ex: ">  file.txt").
 * @return Le nom de fichier extrait (pointeur dans la chaîne d'origine),
 *         ou NULL si la chaîne est vide.
 */
static char *extract_filename(char *str)
{
	if (!str)
		return (NULL);
	while (*str == '<' || *str == '>')
		str++;
	while (*str == ' ' || *str == '\t')
		str++;
	return (str);
}

/**
 * @brief Ouvre le fichier cible pour une redirection selon le type.
 *
 * Gère les modes d'ouverture pour input, output, et append.
 *
 * @param type Type de redirection (REDIR_INPUT, REDIR_OUTPUT, REDIR_APPEND)
 * @param filename Nom du fichier à ouvrir
 * @return Le file descriptor ouvert, ou -1 en cas d'erreur (et affiche perror)
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
 * @brief Applique une redirection sur un file descriptor donné.
 *
 * Utilise dup2 pour rediriger stdin ou stdout selon le type, puis ferme le fd.
 *
 * @param type Type de redirection (REDIR_INPUT, REDIR_OUTPUT, REDIR_APPEND)
 * @param fd File descriptor à appliquer
 * @return 0 en cas de succès, 1 en cas d'erreur (et affiche perror)
 */
static int apply_single_redirection(t_token_type type, int fd)
{
	int result;

	result = 0;
	// Si c'est une redirection d'input, on redirige vers STDIN_FILENO,
	// sinon on redirige vers STDOUT_FILENO.
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
 * @brief Traite un nœud de redirection dans l'AST.
 *
 * Vérifie le type du nœud, extrait le nom de fichier, ouvre le fichier,
 * et applique la redirection correspondante.
 *
 * @param node Nœud AST de redirection à traiter
 * @return 0 si succès, 1 en cas d'erreur ou de nom de fichier manquant
 */
static int process_redirection_node(t_ast *node)
{
	char *filename;
	int fd;

	// Si on n'est pas dans un nœud de redirection valide,
	// on retourne 0 pour indiquer qu'il n'y a pas de redirection à traiter.
	if (node->type != REDIR_INPUT && node->type != REDIR_OUTPUT && node->type != REDIR_APPEND)
		return (0);
	// On extrait le nom de fichier de la chaîne du nœud.
	filename = extract_filename(node->str);
	// Si le nom de fichier est vide ou NULL, on affiche une erreur et on retourne 1.
	if (!filename || *filename == '\0')
	{
		ft_putendl_fd("setup_redirections: missing filename", STDERR_FILENO);
		return (1);
	}
	// On ouvre le fichier selon le type de redirection et on retourne le file descriptor.
	// Si l'ouverture échoue (fd == -1), on affiche une erreur et on retourne 1.
	fd = open_redir_file(node->type, filename);
	if (fd == -1)
		return (1);
	// On applique la redirection en utilisant dup2.
	return (apply_single_redirection(node->type, fd));
}

/**
 * @brief Configure toutes les redirections pour un nœud AST.
 *
 * Parcourt la liste chaînée de nœuds de redirection et applique chaque redirection
 * (input, output, append) dans l'ordre. Affiche une erreur si un nom de fichier
 * est manquant ou si une redirection échoue.
 *
 * @param node Le nœud AST à traiter (doit contenir les redirections à appliquer)
 * @return 0 si toutes les redirections sont configurées avec succès, 1 sinon
 */
int setup_redirections(t_ast *node)
{
	if (!node || !node->str)
	{
		ft_putendl_fd("setup_redirections: invalid node or str",
					  STDERR_FILENO);
		return (1);
	}
	// On parcourt la liste chaînée des 
	while (node)
	{
		// Si on reçoit -1 ou 1, il y'a eu une erreur et on retourn	
		if (process_redirection_node(node) != 0)
			return (1);
		node = node->right;
	}
	return (0);
}
