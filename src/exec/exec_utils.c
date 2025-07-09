/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 08:25:59 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/09 18:07:14 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"

/**
 * @brief Réinitialise les signaux dans un processus enfant.
 *
 * Cette fonction est appelée dans les processus enfants pour réinitialiser
 * les signaux SIGINT et SIGQUIT à leurs actions par défaut.
 */
void	reset_signals_in_child(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

int	is_directory(char *path)
{
	struct stat	path_stat;

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
 * @brief Sauvegarde les descripteurs de fichiers standard (stdin et stdout).
 * 
 * @param saved_stdin Pointeur vers un entier où le descripteur de stdin
 * sera sauvegardé.
 * @param saved_stdout Pointeur vers un entier où le descripteur de stdout
 * sera sauvegardé.
 * @return 0 en cas de succès, 1 en cas d'erreur.
 */
int	save_std_descriptors(int *saved_stdin, int *saved_stdout)
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

void	restore_std_descriptors(int saved_stdin, int saved_stdout)
{
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
}

/**
 * @brief Exécute une commande intégrée avec des redirections,
 * sans gestion d'heredoc.
 * 
 * Cette fonction sauvegarde les descripteurs de fichiers standard,
 * met en place les redirections spécifiées dans l'AST,
 * exécute la commande intégrée,
 * et restaure les descripteurs de fichiers standard.
 * 
 * @param cmd_node Le nœud de commande à exécuter.
 * @param ast_root Le nœud racine de l'AST contenant les redirections
 * @param shell La structure shell contenant l'état du shell.
 * @return Le code de retour de l'exécution de la commande intégrée.
 */
int	exec_builtin_with_redirections_no_heredoc(t_ast *cmd_node, t_ast *ast_root,
		t_shell *shell)
{
	int	saved_stdin;
	int	saved_stdout;
	int	result;

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
