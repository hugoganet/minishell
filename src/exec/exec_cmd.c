/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 16:49:20 by hugoganet         #+#    #+#             */
/*   Updated: 2025/06/12 18:21:00 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Exécute une commande simple à partir d’un noeud AST CMD
 * 
 * Cette fonction crée un processus enfant pour exécuter la commande
 * représentée par le noeud CMD de l'AST. Elle construit d'abord un tableau
 * `argv` à partir du noeud CMD, puis utilise `fork()` pour créer un processus enfant.
 * 
 * Si le processus enfant réussit, il exécutera la commande avec `execve`.
 * Le processus parent attend la fin de l'exécution et récupère le statut de sortie.
 * 
 * @param cmd_node Le noeud CMD de l'AST
 * @param env La structure de l'environnement
 * @return ìnt`
 */
int exec_cmd(t_ast *cmd_node, t_env *env)
{
	pid_t	pid;
	int		status;
	char	**argv;
	char	*path;
	char	**envp;

	// Vérifie que le noeud est bien de type CMD
	if (!cmd_node || cmd_node->type != CMD) 
		return (1);
	// Construit le tableau argv à partir du noeud CMD pour le passer à execve()
	argv = cmd_node->args;
	if (!argv || !argv[0])
		return (1);
	// print_ast_cmd_node(cmd_node->args);
	// Création du processus enfant pour exécuter la commande
	pid = fork();
	if (pid < 0)
	{
		// En cas d'erreur de fork, affiche un message d'erreur
		perror("minishell: fork");
		// Libère la mémoire allouée pour argv
		free(argv);
		// Retourne 1 pour indiquer une erreur
		return (1);
	}
	if (pid == 0)
	{
		// Processus enfant : prépare l'environnement et exécute la commande
		path = resolve_command_path(argv[0], env);
		if (!path)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(argv[0], 2);
			ft_putendl_fd(": command not found", 2);
			exit(127);
		}
		// Si le chemin est trouvé, on prépare l'environnement pour execve
		envp = env_to_char_array(env);
		if (execve(path, argv, envp) == -1)
		{
			perror("minishell: execve");
			exit(126);
		}
		// Si pas d'erreur, le exit ne sera pas atteint
		exit(0);
	}
	// Processus parent : attend la fin du processus enfant et on récupère le statut
	waitpid(pid, &status, 0);
	// Vérifie si le processus enfant s'est terminé normalement
	// Si oui, retourne le code de sortie
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	// Si le processus enfant s'est terminé de manière anormale, on retourne 1
	return (1);
}
