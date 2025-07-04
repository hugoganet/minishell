/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_execution.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 12:44:43 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/04 09:06:22 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"

/**
 * @brief Attend la fin des deux processus fils d'un pipe.
 *
 * @param left_pid PID du processus gauche
 * @param right_pid PID du processus droit
 * @return Code de retour du processus droit
 */
static int wait_for_children(pid_t left_pid, pid_t right_pid)
{
	int status;

	waitpid(left_pid, &status, 0);
	waitpid(right_pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			write(STDOUT_FILENO, "\n", 1);
		else if (WTERMSIG(status) == SIGQUIT)
			write(STDOUT_FILENO, "Quit (core dumped)\n", 20);
		return (128 + WTERMSIG(status));
	}
	return (1);
}

/**
 * @brief Forke et exécute la branche gauche d'un pipe (le producteur).
 *
 * Ce processus écrit dans le pipe. Sa sortie standard (stdout)
 * est redirigée vers l'extrémité d'écriture du pipe.
 *
 * @param fd Le tableau [read, write] du pipe
 * @param left_pid Pointeur vers le PID du processus créé
 * @param node Le nœud de type PIPE contenant la branche gauche
 * @param env Liste chaînée des variables d'environnement
 * @return 0 en cas de succès, 1 sinon
 */
static int execute_left_pid(int fd[2], pid_t *left_pid, t_ast *node, t_env *env, t_shell *shell)
{
	int status;

	*left_pid = fork();
	if (*left_pid < 0)
		return (1);
	if (*left_pid == 0)
	{
		// Redirige stdout vers l'extrémité d'écriture du pipe
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
		status = execute_ast(node->left, env, shell);
		cleanup_shell(shell);
		exit(status);
	}
	return (0);
}

/**
 * @brief Forke et exécute la branche droite d'un pipe (le consommateur).
 *
 * Ce processus lit depuis le pipe. Son entrée standard (stdin)
 * est redirigée vers l'extrémité de lecture du pipe.
 *
 * @param fd Le tableau [read, write] du pipe
 * @param right_pid Pointeur vers le PID du processus créé
 * @param node Le nœud de type PIPE contenant la branche droite
 * @param env Liste chaînée des variables d'environnement
 * @return 0 en cas de succès, 1 sinon
 */
static int execute_right_pid(int fd[2], pid_t *right_pid, t_ast *node, t_env *env, t_shell *shell)
{
	int status;

	*right_pid = fork();
	if (*right_pid < 0)
		return (1);
	if (*right_pid == 0)
	{
		// Redirige stdin depuis l'extrémité de lecture du pipe
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		close(fd[1]);
		status = execute_ast(node->right, env, shell);
		cleanup_shell(shell);
		exit(status);
	}
	return (0);
}

/**
 * @brief Exécute un nœud de type PIPE de l'AST.
 *
 * Crée un pipe, fork deux processus (gauche et droit),
 * connecte leur stdin/stdout via le pipe, puis attend leur fin.
 *
 * @param node Le nœud de type PIPE
 * @param env Liste chaînée des variables d'environnement
 * @param shell Structure principale du shell
 * @return Code de retour du processus droit (dernière commande du pipe)
 */
int execute_pipe_node(t_ast *node, t_env *env, t_shell *shell)
{
	int fd[2];
	pid_t left_pid, right_pid;

	if (pipe(fd) == -1)
	{
		perror("minishell: pipe");
		return (1);
	}
	if (execute_left_pid(fd, &left_pid, node, env, shell) != 0)
	{
		close_pipe_fds(fd);
		return (1);
	}
	if (execute_right_pid(fd, &right_pid, node, env, shell) != 0)
	{
		close_pipe_fds(fd);
		return (1);
	}
	close_pipe_fds(fd);
	return (wait_for_children(left_pid, right_pid));
}
