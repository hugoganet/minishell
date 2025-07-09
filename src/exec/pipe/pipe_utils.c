/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 16:00:00 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/09 14:27:06 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file pipe_utils.c
 * @brief Utilitaires pour le traitement des commandes pipeline et la gestion des ressources
 * 
 * Ce module fournit des fonctions d'aide pour gérer les opérations de pipeline,
 * incluant le comptage des commandes, l'extraction, l'allocation de ressources et la création de pipes.
 * Ces utilitaires supportent le système d'exécution complexe de pipeline dans minishell.
 * 
 * @author hugoganet
 * @date 2025-07-07
 */

#include "pipe.h"

/**
 * @brief Compte le nombre total de commandes dans un AST de pipeline
 * 
 * Cette fonction parcourt récursivement l'AST pour compter tous les nœuds de commande
 * dans une structure de pipeline. Elle gère à la fois les nœuds CMD directs et les nœuds
 * qui contiennent des commandes (comme les nœuds de redirection avec commandes intégrées).
 * 
 * @param node Le nœud AST à analyser (peut être NULL)
 * @return Le nombre total de commandes dans le pipeline (0 si node est NULL) 
 * 
 * @note Cette fonction est utilisée pour déterminer les besoins d'allocation de ressources
 *       pour l'exécution du pipeline (pipes, IDs de processus, etc.)
 */
int	count_pipeline_commands(t_ast *node)
{
	if (!node)
		return (0);
	if (node->type == CMD)
		return (1);
	if (node->type == PIPE)
	// Si le nœud est un PIPE, on compte les COMMANDES dans les sous-nœuds en rappelant récursivement
	// les deux branches gauche et droite du pipeline
	return (count_pipeline_commands(node->left)
			+ count_pipeline_commands(node->right));
	// Si ce n'est ni un CMD ni un PIPE, c'est un nœud de redirection
	// (REDIR_INPUT, REDIR_OUTPUT, REDIR_APPEND, HEREDOC), on cherche alors le 1er nœud CMD
	// dans le sous-arbre gauche ou droit, et on ajout 1 au compteur de CMD qui sera retourné
	if (find_cmd_node(node))
		return (1);
	return (0);
}

/**
 * @brief Extrait tous les nœuds de commande d'un AST de pipeline dans un tableau
 * 
 * Cette fonction effectue un parcours en profondeur de l'AST de pipeline et
 * extrait tous les nœuds de commande dans un tableau linéaire. Elle gère à la fois
 * les nœuds CMD directs et les nœuds composites contenant des commandes (comme les redirections).
 * 
 * @param node Le nœud AST à traiter (peut être NULL)
 * @param commands Tableau pour stocker les nœuds de commande extraits
 * @param index Pointeur vers l'index actuel dans le tableau commands (modifié par la fonction)
 * 
 * @note Le paramètre index est passé par référence et est incrémenté
 *       chaque fois qu'une commande est ajoutée au tableau
 * @note Pour les nœuds non-CMD, le nœud entier est stocké s'il contient une commande
 */
void	extract_pipeline_commands(t_ast *node, t_ast **commands, int *index)
{
	t_ast	*cmd_node;

	if (!node)
		return ;
	if (node->type == PIPE)
	{
		extract_pipeline_commands(node->left, commands, index);
		extract_pipeline_commands(node->right, commands, index);
		return ;
	}
	if (node->type == CMD)
	{
		commands[*index] = node;
		(*index)++;
		return ;
	}
	cmd_node = find_cmd_node(node);
	if (cmd_node)
	{
		commands[*index] = node;
		(*index)++;
	}
}

/**
 * @brief Détermine si un pipeline est complexe (plus de 2 commandes)
 * 
 * Cette fonction vérifie si un pipeline contient plus de 2 commandes,
 * ce qui détermine la stratégie d'exécution. Les pipelines complexes nécessitent
 * un traitement différent des pipes simples à deux commandes.
 * 
 * @param node Le nœud AST de pipeline à analyser
 * @return true si le pipeline a plus de 2 commandes, false sinon
 * 
 * @note Les pipelines complexes (>2 commandes) utilisent l'algorithme d'exécution
 *       complexe de pipeline, tandis que les pipelines simples utilisent l'exécution binaire optimisée
 */
bool	is_complex_pipeline(t_ast *node)
{
	// Retourne true si le nombre de commandes dans le pipeline est supérieur à 2
	return (count_pipeline_commands(node) > 2);
}

/**
 * @brief Alloue toutes les ressources nécessaires pour l'exécution du pipeline
 * 
 * Cette fonction alloue la mémoire pour les trois principaux tableaux de ressources nécessaires
 * pour l'exécution du pipeline : tableau des nœuds de commande, tableau des pipes et tableau des IDs de processus.
 * Elle effectue un nettoyage approprié en cas d'échec d'allocation.
 * 
 * @param cmd_count Nombre de commandes dans le pipeline
 * @param commands Pointeur vers le pointeur du tableau de commandes (alloué par la fonction)
 * @param pipes Pointeur vers le pointeur du tableau de pipes (alloué par la fonction)
 * @param pids Pointeur vers le pointeur du tableau d'IDs de processus (alloué par la fonction)
 * @return 0 en cas de succès, 1 en cas d'échec d'allocation
 * 
 * @note Tous les pointeurs alloués sont définis via les paramètres de double pointeur
 * @note En cas d'échec, toute mémoire allouée avec succès est libérée avant le retour
 * @note Le tableau pipes a (cmd_count - 1) éléments car n commandes nécessitent (n-1) pipes
 */
int	allocate_pipeline_resources(int cmd_count, t_ast ***commands,
		int ***pipes, pid_t **pids)
{
	*commands = malloc(cmd_count * sizeof(t_ast *));
	if (!*commands)
		return (1);
	*pipes = malloc((cmd_count - 1) * sizeof(int *));
	if (!*pipes)
	{
		free(*commands);
		return (1);
	}
	*pids = malloc(cmd_count * sizeof(pid_t));
	if (!*pids)
	{
		free(*commands);
		free(*pipes);
		return (1);
	}
	return (0);
}

/**
 * @brief Crée tous les pipes nécessaires pour l'exécution du pipeline
 * 
 * Cette fonction crée tous les pipes requis pour un pipeline en allouant
 * la mémoire pour chaque paire de descripteurs de fichier et en appelant l'appel système pipe().
 * Elle initialise d'abord le tableau pipes à NULL, puis crée chaque pipe séquentiellement.
 * 
 * @param pipes Tableau de pointeurs int pour stocker les descripteurs de fichier des pipes
 * @param cmd_count Nombre de commandes dans le pipeline
 * @return Nombre de pipes créés avec succès, ou index du premier échec
 * 
 * @note Pour n commandes, (n-1) pipes sont nécessaires
 * @note En cas d'échec, la fonction retourne l'index où la création a échoué
 * @note Chaque pipes[i] pointe vers un tableau de 2 entiers [read_fd, write_fd]
 * @note Le tableau pipes est initialisé à NULL avant la création des pipes
 * @note En cas d'échec de l'appel système pipe(), affiche l'erreur et nettoie
 */
int	create_all_pipes(int **pipes, int cmd_count)
{
	int	i;

	i = 0;
	while (i < cmd_count - 1)
	{
		pipes[i] = NULL;
		i++;
	}
	i = 0;
	while (i < cmd_count - 1)
	{
		pipes[i] = ft_calloc(2, sizeof(int));
		if (!pipes[i])
			return (i);
		if (pipe(pipes[i]) == -1)
		{
			perror("minishell: pipe");
			free(pipes[i]);
			return (i);
		}
		i++;
	}
	return (cmd_count - 1);
}
