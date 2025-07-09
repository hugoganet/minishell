/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 08:32:17 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/09 18:32:18 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <errno.h>

/**
 * @brief Recherche récursivement un nœud de commande dans l'AST
 * 
 * Cette fonction parcourt l'AST de manière récursive pour trouver le premier
 * nœud de type CMD. Elle effectue une recherche en profondeur d'abord (DFS)
 * en explorant d'abord le sous-arbre gauche, puis le sous-arbre droit.
 * 
 * @param node Le nœud AST à analyser (peut être NULL)
 * @return Pointeur vers le premier nœud CMD trouvé, ou NULL si aucun trouvé
 */
t_ast	*find_cmd_node(t_ast *node)
{
	t_ast	*found;

	if (!node)
		return (NULL);
	if (node->type == CMD)
		return (node);
	found = find_cmd_node(node->left);
	if (found)
		return (found);
	return (find_cmd_node(node->right));
}

/**
 * @brief Résout et vérifie le chemin d'une commande avant exécution
 * 
 * Cette fonction résout le chemin complet d'une commande en utilisant la
 * variable d'environnement PATH, puis vérifie que le chemin résolu n'est pas
 * un répertoire. Elle gère les erreurs en nettoyant les ressources et en sortant
 * avec le code approprié.
 * 
 * @param argv Tableau d'arguments de la commande (argv[0] est le nom de
 * la commande)
 * @param env Environnement pour la résolution du PATH
 * @param shell Structure shell pour le nettoyage en cas d'erreur
 * @return Chemin résolu de la commande (doit être libéré par l'appelant)
 * 
 * @note Cette fonction ne retourne jamais en cas d'erreur (appelle exit())
 * @note Code de sortie 127 pour commande non trouvée
 * @note Code de sortie 126 pour répertoire au lieu d'exécutable
 * @note Effectue un nettoyage complet des ressources avant exit()
 */
static char	*resolve_and_check_command(char **argv, t_env *env, t_shell *shell)
{
	char	*path;

	path = resolve_command_path(argv[0], env);
	if (!path)
	{
		print_command_not_found_error(argv[0]);
		cleanup_shell(shell);
		exit(127);
	}
	if (is_directory(path))
	{
		free(path);
		cleanup_shell(shell);
		exit(126);
	}
	return (path);
}

/**
 * @brief Exécute une commande dans un processus enfant
 * 
 * Cette fonction configure et exécute une commande externe dans un
 * processus enfant.Elle gère la configuration des signaux, les redirections
 * heredoc et d'I/O, la résolution du chemin de la commande, et
 * l'appel final à execve().
 * 
 * @param argv Tableau d'arguments de la commande (terminé par NULL)
 * @param env Environnement pour la résolution du PATH et l'exécution
 * @param ast Nœud AST contenant les informations de redirection
 * @param shell Structure shell pour le nettoyage et les heredocs
 * 
 * @note Cette fonction ne retourne jamais en cas de succès (remplace
 * le processus)
 * @note En cas d'échec, nettoie les ressources et sort avec un code d'erreur
 * @note Les codes de sortie suivent la convention bash :
 *       - 1 : échec de redirection
 *       - 126 : permission refusée ou répertoire
 *       - 127 : commande non trouvée ou échec execve
 * @note Ignore les erreurs ENOEXEC pour éviter les messages d'erreur redondants
 */
void	run_child_process(char **argv, t_env *env,
		t_ast *ast, t_shell *shell)
{
	char	*path;
	char	**envp;

	reset_signals_in_child();
	setup_heredoc_redirection(shell);
	if (setup_redirections(ast) != 0)
	{
		cleanup_shell(shell);
		exit(1);
	}
	path = resolve_and_check_command(argv, env, shell);
	envp = env_to_char_array(env);
	if (!envp)
	{
		perror("minishell: env malloc");
		cleanup_shell(shell);
		exit(1);
	}
	if (execve(path, argv, envp) == -1)
	{
		if (errno != ENOEXEC)
			perror("minishell: execve");
		free_child_exec(path, envp, shell);
		exit(127);
	}
}
