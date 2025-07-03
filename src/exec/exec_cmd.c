/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 16:49:20 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/03 22:18:24 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/stat.h>

/*
** @brief Filtre les arguments vides d'un tableau de chaînes.
**
** Cette fonction modifie le tableau sur place pour enlever les chaînes
** vides (résultant de l'expansion de variables inexistantes).
** Elle libère la mémoire des chaînes vides pour éviter les fuites.
**
** @param args Le tableau d'arguments à filtrer.
*/
static void filter_empty_args(char **args)
{
	int i;
	int j;

	i = 0;
	j = 0;
	if (!args)
		return;
	while (args[i])
	{
		if (args[i][0] == '\0')
		{
			free(args[i]);
		}
		else
		{
			if (i != j)
				args[j] = args[i];
			j++;
		}
		i++;
	}
	args[j] = NULL;
}

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

static void free_child_exec(char *path, char **envp, t_shell *shell)
{
	free(path);
	free_env(envp);
	cleanup_shell(shell);
}

static void run_child_process(char **argv, t_env *env,
							  t_ast *ast, t_shell *shell)
{
	char *path;
	char **envp;
	struct stat path_stat;

	reset_signals_in_child();

	// Si un heredoc a été traité, on redirige l'entrée standard
	if (shell->heredoc_fd != -1)
	{
		if (dup2(shell->heredoc_fd, STDIN_FILENO) == -1)
			perror("minishell: dup2 heredoc");
		close(shell->heredoc_fd);
		shell->heredoc_fd = -1;
	}

	if (setup_redirections(ast) != 0)
	{
		cleanup_shell(shell);
		exit(1);
	}
	path = resolve_command_path(argv[0], env);
	if (!path)
	{
		if (ft_strchr(argv[0], '/'))
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			perror(argv[0]);
		}
		else
		{
			ft_putstr_fd(argv[0], STDERR_FILENO);
			ft_putendl_fd(": command not found", STDERR_FILENO);
		}
		cleanup_shell(shell);
		exit(127);
	}
	if (stat(path, &path_stat) == 0)
	{
		if (S_ISDIR(path_stat.st_mode))
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(path, STDERR_FILENO);
			ft_putendl_fd(": is a directory", STDERR_FILENO);
			free(path);
			cleanup_shell(shell);
			exit(126);
		}
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
		perror("minishell: execve");
		free_child_exec(path, envp, shell);
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
 *
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
	t_ast *tmp;
	int heredoc_status;

	cmd_node = find_cmd_node(ast_root);
	tmp = ast_root;
	heredoc_status = 0;
	while (tmp)
	{
		if (tmp->type == HEREDOC)
		{
			heredoc_status = handle_heredoc(tmp->str, shell);
			if (heredoc_status == 130) // SIGINT reçu pendant heredoc
			{
				if (shell->heredoc_fd != -1)
					close(shell->heredoc_fd);
				shell->heredoc_fd = -1;
				return (130);
			}
		}
		tmp = tmp->right;
	}
	if (!cmd_node || !cmd_node->args)
		return (0);
	filter_empty_args(cmd_node->args);
	if (!cmd_node->args[0])
		return (0);
	if (is_builtin(cmd_node))
		return (builtin_exec(cmd_node, shell));
	pid = fork();
	if (pid < 0)
	{
		perror("minishell: fork");
		return (1);
	}
	if (pid == 0)
		run_child_process(cmd_node->args, env, ast_root, shell);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	waitpid(pid, &status, 0);
	init_signals();
	if (shell->heredoc_fd != -1)
	{
		close(shell->heredoc_fd);
		shell->heredoc_fd = -1;
	}
	return (handle_child_status(status));
}

void free_string_array(char **arr)
{
	if (!arr)
		return;
	for (int i = 0; arr[i]; i++)
	{
		free(arr[i]);
	}
	free(arr);
}
