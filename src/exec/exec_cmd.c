/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hganet <hganet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 16:49:20 by hugoganet         #+#    #+#             */
/*   Updated: 2025/06/24 17:56:15 by hganet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Cherche récursivement le premier noeud de type CMD.
 *
 * @param node Nœud racine du sous-arbre AST.
 * @return Pointeur vers le noeud de type CMD, ou NULL si non trouvé.
 */
t_ast *find_cmd_node(t_ast *node)
{
	t_ast *found;

	// Si le noeud est nul, on ne trouve rien
	if (!node)
		return (NULL);
	// Si le noeud courant est un CMD, on le retourne
	if (node->type == CMD)
		return (node);
	// Recherche récursive dans la branche gauche
	found = find_cmd_node(node->left);
	if (found)
		return (found);
	// Sinon, recherche dans la branche droite
	return (find_cmd_node(node->right));
}

/**
 * @brief Initialise les signaux dans le processus enfant.
 *
 * Permet à l'enfant de recevoir normalement les signaux SIGINT et SIGQUIT.
 */
static void reset_signals_in_child(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

/**
 * @brief Exécute une commande dans un processus enfant.
 *
 * - Applique les redirections
 * - Résout le path de la commande
 * - Convertit l'env en tableau char **
 * - Lance execve
 *
 * @param argv Tableau d'arguments
 * @param env Liste chaînée de l'environnement
 * @param ast Arbre AST courant (pour les redirections)
 * @param shell Structure principale du shell (pour cleanup)
 */
static void run_child_process(char **argv, t_env *env,
							  t_ast *ast, t_shell *shell)
{
	char *path;
	char **envp;

	// Active les signaux par défaut dans le processus enfant
	reset_signals_in_child();
	// Si le nœud actuel est un heredoc, on applique la redirection stdin
	if (ast->type == HEREDOC)
		handle_heredoc(ast->str);
	// Applique toutes les autres redirections (> >> <)
	if (setup_redirections(ast) != 0)
	{
		cleanup_shell(shell);
		exit(1);
	}
	// Résout le chemin absolu vers l'exécutable
	path = resolve_command_path(argv[0], env);
	if (!path)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(argv[0], STDERR_FILENO);
		ft_putendl_fd(": command not found", STDERR_FILENO);
		cleanup_shell(shell);
		exit(127);
	}

	// Construit le tableau d'environnement (char **)
	envp = env_to_char_array(env);
	if (!envp)
	{
		perror("minishell: env malloc");
		cleanup_shell(shell);
		exit(1);
	}

	// Exécute la commande avec execve
	if (execve(path, argv, envp) == -1)
	{
		perror("minishell: execve");
		cleanup_shell(shell);
		exit(126);
	}
}

/**
 * @brief Gère le code retour du processus enfant après un waitpid.
 *
 * @param status Statut renvoyé par waitpid
 * @return Code de sortie du shell pour cette commande
 */
static int handle_child_status(int status)
{
	// Si l'enfant a été tué par un signal
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			write(STDOUT_FILENO, "\n", 1);
		else if (WTERMSIG(status) == SIGQUIT)
			write(STDOUT_FILENO, "Quit (core dumped)\n", 20);
		return (128 + WTERMSIG(status));
	}
	// Si l'enfant s'est terminé normalement
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	// Par défaut
	return (1);
}

/**
 * @brief Exécute une commande simple (non composée) en forkant un enfant.
 *
 * - Récupère le noeud CMD
 * - Prépare argv
 * - Fork, exécute, puis attend la fin du processus enfant
 *
 * @param cmd_node Le noeud CMD (ou un noeud supérieur contenant le CMD)
 * @param env Liste des variables d'environnement
 * @param ast_root Racine de l'AST courant
 * @param shell Structure du shell principal
 * @return Code de retour de la commande exécutée
 */
int exec_cmd(t_ast *cmd_node, t_env *env, t_ast *ast_root, t_shell *shell)
{
	pid_t pid;
	int status;
	char **argv;

	// Recherche du vrai noeud CMD à exécuter
	cmd_node = find_cmd_node(ast_root);
	if (!cmd_node || !cmd_node->args || !cmd_node->args[0])
		return (1);
	if (is_builtin(cmd_node))
		shell->last_exit_status = builtin_exec(cmd_node, shell);	
	else
	{
		argv = cmd_node->args;
	
		// Fork du processus
		pid = fork();
		if (pid < 0)
		{
			perror("minishell: fork");
			return (1);
		}
		// Enfant : exécute la commande
		if (pid == 0)
			run_child_process(argv, env, ast_root, shell);
		// Parent : ignore temporairement SIGINT et SIGQUIT
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		// Attend la fin du processus enfant
		waitpid(pid, &status, 0);
		// Réactive les signaux du shell (readline)
		init_signals();
		// Gère le code de retour du processus
		shell->last_exit_status = handle_child_status(status);
	}
	
	return (shell->last_exit_status);
}
