/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 16:49:20 by hugoganet         #+#    #+#             */
/*   Updated: 2025/06/11 16:01:14 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Compte le nombre d'arguments à partir d'un noeud CMD
 * 
 * Cette fonction parcourt l'AST à partir du noeud CMD et compte le nombre
 * d'arguments (`CMD` et `ARG`) jusqu'à ce qu'elle atteigne un noeud de type différent.
 * 
 * @param cmd_node Pointer vers le noeud CMD de l'AST
 * @return `int` Le nombre d'arguments trouvés
 */
static int count_args(t_ast *cmd_node)
{
	t_ast	*curr;
	int		count;

	// Set le pointer courant sur le pointer reçu en argument
	curr = cmd_node;
	// Initialise le compteur d'arguments à 0
	count = 0;
	// Parcourt l'AST tant que le type est CMD ou ARG
	while (curr && (curr->type == CMD || curr->type == ARG))
	{
		// Incrémente le nombre d'argument à chaque itération
		count++;
		// Set le pointer courant sur le noeud de droite pour accéder au prochain argument
		curr = curr->right;
	}
	return (count);
}

/**
 * @brief Construit le tableau `char **argv` à partir d’un noeud CMD
 * 
 * Cette fonction parcourt l'AST à partir du noeud CMD et extrait les
 * arguments (`CMD` et `ARG`) pour les stocker dans un tableau de chaînes de caractères.
 * C'est essentiel pour pouvoir exécuter la commande avec `execve`.
 * 
 * @param cmd_node Pointer vers le noeud CMD de l'AST
 * @return `char **` Un tableau de chaînes de caractères contenant les arguments
 */
static char **build_argv(t_ast *cmd_node)
{
	int		i;
	int		count;
	t_ast	*curr;
	char	**argv;

	// Compte le nombre d'arguments à partir du noeud CMD
	count = count_args(cmd_node);
	if (count == 0)
		return (NULL);
	// Alloue de la mémoire pour le tableau argv
	argv = ft_calloc(sizeof(char *), (count + 1));
	if (!argv)
		return (NULL);
	// Set le pointer courant sur le noeud CMD
	curr = cmd_node;
	i = 0;
	// Parcourt l'AST et remplit le tableau argv avec les chaînes de caractères
	while (curr && (curr->type == CMD || curr->type == ARG))
	{
		// Assigne la chaîne de caractères du noeud courant à argv
		argv[i++] = curr->str;
		// Avance au noeud de droite pour accéder au prochain argument
		curr = curr->right;
	}
	return (argv);
}

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

	// printf("\nadresse de t_env dans le parent : %p\n", (void *)env);
	// Vérifie que le noeud est bien de type CMD
	if (!cmd_node || cmd_node->type != CMD) return (1);
	// Construit le tableau argv à partir du noeud CMD
	// pour le passer à execve()
	argv = build_argv(cmd_node);
	if (!argv || !argv[0])
		return (1);
	// Affiche la commande pour le debug
	// print_ast_cmd_node(argv);
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
			exit(126); // 126 = erreur lors de l’exécution
		}
		// Si pas d'erreur, le exit ne sera pas atteint
		exit(0);
	}
	// Processus parent : attend la fin du processus enfant et on récupère le statut
	waitpid(pid, &status, 0);
	// Libère la mémoire allouée pour argv
	free(argv);
	// Vérifie si le processus enfant s'est terminé normalement
	// Si oui, retourne le code de sortie
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	// Si le processus enfant s'est terminé de manière anormale, on retourne 1
	return (1);
}
