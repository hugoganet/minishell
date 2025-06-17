/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 16:49:20 by hugoganet         #+#    #+#             */
/*   Updated: 2025/06/17 14:33:44 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Cherche récursivement le nœud CMD à partir d’un sous-arbre.
 *
 * @param node Nœud racine potentiel (redirection, etc.)
 * @return Pointeur vers le nœud CMD, ou NULL s’il n’est pas trouvé
 */
t_ast *find_cmd_node(t_ast *node)
{
	t_ast *found;
	
	found = NULL;
	if (!node)
		return NULL;
	if (node->type == CMD)
		return node;
	// Cherche dans la branche gauche
	 found = find_cmd_node(node->left);
	if (found)
		return (found);
	// Cherche dans la branche droite
	return (find_cmd_node(node->right));
}

/**
 * @brief Lance le processus enfant : redirections + execve
 *
 * Cette fonction est appelée dans le processus enfant après un fork.
 * Elle applique les redirections, résout le chemin de la commande,
 * puis exécute la commande via execve.
 *
 * @param argv Tableau d'arguments de la commande
 * @param env Liste chaînée des variables d'environnement
 * @param ast_root Racine du sous-arbre (pour les redirections)
 */
static void run_child_process(char **argv, t_env *env, t_ast *ast_root, t_shell *shell)
{
	char *path;
	char **envp;

	if (setup_redirections(ast_root) != 0)
		exit(1);
	path = resolve_command_path(argv[0], env);
	// printf("run_child_process - PATH: %s\n", path ? path : "NULL");
	// fflush(stdout);
	if (!path)
	{
		// ! Attention ici parce que ça exit sans free les ressources
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(argv[0], 2);
		ft_putendl_fd(": command not found", 2);
		cleanup_shell(shell);
		exit(127);
	}
	envp = env_to_char_array(env);
	if (!envp)
	{
		perror("minishell: env malloc");
		cleanup_shell(shell);
		exit(1);
	}
	if (execve(path, argv, envp) == -1)
	{
		// ? il faut peut-être free tous les pointeurs avant de quitter vu qu'on est dans un fork
		perror("minishell: execve");
		cleanup_shell(shell);
		exit(126);
	}
}

/**
 * @brief Exécute une commande simple à partir d’un noeud AST CMD
 *
 * Cette fonction fork un processus enfant, lui transmet les redirections
 * et les arguments, puis exécute la commande via execve.
 *
 * @param cmd_node Le noeud CMD contenant les arguments
 * @param env Liste des variables d'environnement
 * @param ast_root Racine de l'AST
 * @return Code de retour de la commande
 */
int exec_cmd(t_ast *cmd_node, t_env *env, t_ast *ast_root, t_shell *shell)
{
	pid_t pid;
	int status;
	char **argv;

	cmd_node = find_cmd_node(ast_root);
	// printf("exec_cmd - AST Node Type: %d\n", cmd_node->type);
	if (!cmd_node)
	{
		ft_putendl_fd("exec_cmd: CMD node introuvable !", 2);
		return (1);
	}
	argv = cmd_node->args;
	if (!argv || !argv[0])
		return (1);
	pid = fork();
	if (pid < 0)
	{
		perror("minishell: fork");
		return (1);
	}
	if (pid == 0)
		run_child_process(argv, env, ast_root, shell);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}
