/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 12:44:43 by hugoganet         #+#    #+#             */
/*   Updated: 2025/06/10 14:58:50 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Attend la fin des deux processus fils d’un pipe.
 *
 * @param left_pid PID du processus gauche
 * @param right_pid PID du processus droit
 * @return Code de retour du processus droit
 */
static int wait_for_children(pid_t left_pid, pid_t right_pid)
{
	int status;

	waitpid(left_pid, NULL, 0);
	waitpid(right_pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

/**
 * @brief Ferme les deux extrémités d’un pipe.
 *
 * @param fds Tableau contenant les descripteurs [lecture, écriture]
 */
static void close_pipe_fds(int fds[2])
{
	close(fds[0]);
	close(fds[1]);
}

/**
 * @brief Forke et exécute la branche gauche d’un pipe (le producteur).
 *
 * Ce processus écrit dans le pipe. Sa sortie standard (stdout)
 * est redirigée vers l'extrémité d'écriture du pipe.
 *
 * @param fd Le tableau [read, write] du pipe
 * @param left_pid Pointeur vers le PID du processus créé
 * @param node Le nœud de type PIPE contenant la branche gauche
 * @param env Liste chaînée des variables d’environnement
 * @return 0 en cas de succès, 1 sinon
 */
static int execute_left_pid(int fd[2], pid_t *left_pid, t_ast *node, t_env *env)
{
	*left_pid = fork();
	if (*left_pid < 0)
		return (1);
	if (*left_pid == 0)
	{
		printf("\nadresse de t_env dans l'enfant gauche : %p\n", (void *)env);
		fflush(stdout);
		// Redirige stdout vers l’extrémité d’écriture du pipe
		if (dup2(fd[1], STDOUT_FILENO) == -1)
		{
			perror("minishell: dup2");
			exit(1);
		}
		// Ferme les deux extrémités du pipe (déjà dupliquées)
		close_pipe_fds(fd);
		// Exécute récursivement la branche gauche
		exit(execute_ast(node->left, env));
	}
	return (0);
}

/**
 * @brief Forke et exécute la branche droite d’un pipe (le consommateur).
 *
 * Ce processus lit depuis l'extrémité de lecture du pipe.
 *
 * @param fd Le tableau [read, write] du pipe
 * @param right_pid Pointeur vers le PID du processus créé
 * @param node Le nœud de type PIPE contenant la branche droite
 * @param env Liste chaînée des variables d’environnement
 * @return 0 en cas de succès, 1 sinon
 */
static int execute_right_pid(int fd[2], pid_t *right_pid, t_ast *node, t_env *env)
{
	*right_pid = fork();
	if (*right_pid < 0)
		return (1);
	if (*right_pid == 0)
	{
		printf("\nadresse de t_env dans l'enfant droit : %p\n", (void *)env);
		fflush(stdout);
		// Redirige stdin vers l’extrémité de lecture du pipe
		if (dup2(fd[0], STDIN_FILENO) == -1)
		{
			perror("minishell: dup2");
			exit(1);
		}
		// Ferme les deux extrémités du pipe (déjà dupliquées)
		close_pipe_fds(fd);
		// Exécute récursivement la branche droite
		exit(execute_ast(node->right, env));
	}
	return (0);
}

/**
 * @brief Exécute un nœud PIPE de l'AST.
 *
 * Cette fonction connecte deux processus via un pipe :
 * - `node->left` écrit (stdout) dans le pipe
 * - `node->right` lit (stdin) depuis le pipe
 *
 * Le code de sortie du processus de droite est retourné.
 *
 * @param node Le nœud AST de type PIPE
 * @param env Liste chaînée des variables d’environnement
 * @return Code de sortie du processus de droite
 */
int execute_pipe_node(t_ast *node, t_env *env)
{
	int		fd[2];
	pid_t	left_pid;
	pid_t	right_pid;

	printf("\nadresse de t_env dans le parent : %p\n", (void *)env);
	// Création du pipe
	if (pipe(fd) == -1)
	{
		perror("minishell: pipe");
		return (1);
	}
	// Lancer le processus gauche
	if (execute_left_pid(fd, &left_pid, node, env) != 0)
	{
		close_pipe_fds(fd);
		return (1);
	}
	// Lancer le processus droit
	if (execute_right_pid(fd, &right_pid, node, env) != 0)
	{
		close_pipe_fds(fd);
		kill(left_pid, SIGTERM);
		return (1);
	}
	// Le parent ferme les deux extrémités du pipe
	close_pipe_fds(fd);
	// Attend les deux processus enfants et retourne le code de sortie
	// du pid gauche ou 1 en cas d'erreur
	return (wait_for_children(left_pid, right_pid));
}
