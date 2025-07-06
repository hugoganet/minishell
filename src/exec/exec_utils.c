/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 08:25:59 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/06 12:06:21 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"

/**
 * @brief Initialise les signaux dans le processus enfant.
 *
 * Permet à l'enfant de recevoir normalement les signaux SIGINT et SIGQUIT.
 */
void reset_signals_in_child(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

/**
 * @brief Affiche le message d'erreur approprié pour une commande non trouvée.
 *
 * @param cmd_name Nom de la commande
 */
void print_command_not_found_error(char *cmd_name)
{
	if (ft_strchr(cmd_name, '/'))
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		perror(cmd_name);
	}
	else
	{
		ft_putstr_fd(cmd_name, STDERR_FILENO);
		ft_putendl_fd(": command not found", STDERR_FILENO);
	}
}

/**
 * @brief Vérifie si le chemin pointe vers un répertoire.
 *
 * @param path Chemin à vérifier
 * @return 1 si c'est un répertoire, 0 sinon
 */
int is_directory(char *path)
{
	struct stat path_stat;

	if (stat(path, &path_stat) == 0)
	{
		if (S_ISDIR(path_stat.st_mode))
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(path, STDERR_FILENO);
			ft_putendl_fd(": is a directory", STDERR_FILENO);
			return (1);
		}
	}
	return (0);
}

/**
 * @brief Sauvegarde les descripteurs de fichier stdin et stdout.
 *
 * @param saved_stdin Pointeur pour stocker le descripteur stdin sauvegardé
 * @param saved_stdout Pointeur pour stocker le descripteur stdout sauvegardé
 * @return 0 en cas de succès, 1 en cas d'erreur
 */
int save_std_descriptors(int *saved_stdin, int *saved_stdout)
{
	*saved_stdin = dup(STDIN_FILENO);
	*saved_stdout = dup(STDOUT_FILENO);
	if (*saved_stdin == -1 || *saved_stdout == -1)
	{
		perror("dup");
		return (1);
	}
	return (0);
}

/**
 * @brief Restaure les descripteurs de fichier originaux et les ferme.
 *
 * @param saved_stdin Descripteur stdin sauvegardé
 * @param saved_stdout Descripteur stdout sauvegardé
 */
void restore_std_descriptors(int saved_stdin, int saved_stdout)
{
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
}

/**
 * @brief Exécute un builtin avec redirections sans traiter les heredocs.
 *
 * Version utilisée dans les processus enfants de pipe où les heredocs
 * ont déjà été traités par le processus parent.
 *
 * @param cmd_node Le nœud contenant la commande builtin
 * @param ast_root Le nœud racine contenant les redirections
 * @param shell Structure du shell principal
 * @return Code de retour du builtin exécuté
 */
int exec_builtin_with_redirections_no_heredoc(t_ast *cmd_node, t_ast *ast_root,
											  t_shell *shell)
{
	int saved_stdin;
	int saved_stdout;
	int result;

	if (save_std_descriptors(&saved_stdin, &saved_stdout) != 0)
		return (1);
	if (setup_redirections(ast_root) != 0)
	{
		restore_std_descriptors(saved_stdin, saved_stdout);
		return (1);
	}
	result = builtin_exec(cmd_node, shell);
	restore_std_descriptors(saved_stdin, saved_stdout);
	return (result);
}
