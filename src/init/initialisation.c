/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialisation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 13:28:30 by elaudrez          #+#    #+#             */
/*   Updated: 2025/07/07 14:55:29 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Vérifie si SHLVL existe dans l'environnement envp
 * @param envp Le tableau d'environnement
 * @return int 1 si SHLVL existe, 0 sinon
 */
static int	shlvl_exists_in_envp(char **envp)
{
	int	i;

	if (!envp)
		return (0);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "SHLVL=", 6) == 0)
			return (1);
		i++;
	}
	return (0);
}

/**
 * @brief S'assure que SHLVL est défini à 1 (pour le shell racine)
 * @param env_list La liste d'environnement
 * @return int 0 en cas de succès, 1 en cas d'erreur
 */
static int	ensure_shlvl_is_one(t_env *env_list)
{
	t_env	*shlvl_node;
	char	*new_value;

	// Rechercher le nœud SHLVL
	shlvl_node = env_list;
	while (shlvl_node)
	{
		if (ft_strcmp(shlvl_node->key, "SHLVL") == 0)
			break;
		shlvl_node = shlvl_node->next;
	}
	
	// Si SHLVL n'existe pas, le créer avec la valeur "1"
	if (!shlvl_node)
	{
		shlvl_node = create_env_pair("SHLVL", "1");
		if (!shlvl_node)
			return (1);
		// Ajouter le nouveau nœud à la liste
		shlvl_node->next = env_list->next;
		env_list->next = shlvl_node;
		return (0);
	}
	
	// Si SHLVL existe déjà, s'assurer qu'il est à 1
	new_value = ft_strdup("1");
	if (!new_value)
		return (1);
	free(shlvl_node->value);
	shlvl_node->value = new_value;
	return (0);
}

/**
 * @brief Initialise la structure du shell : copie de l'environnement,
 *        initialisation des signaux et du statut de sortie.
 *
 * @param shell Pointeur vers la structure à initialiser.
 * @param envp Environnement système (non modifiable directement).
 * @param env_list Liste chaînée d'environnement (initialisée dans init_env_list).
 */
void init_shell(t_shell *shell, char **envp, t_env *env_list)
{
	// Initialisation à NULL des champs de la structure shell
	shell->env = NULL;
	shell->env_list = NULL;
	shell->export_list = NULL;
	shell->tokens = NULL;
	shell->ast = NULL;
	shell->last_exit_status = 0;
	shell->heredoc_fds = NULL;// Liste chaînée des descripteurs de fichier heredoc
	// Copie de l'environnement dans la structure shell
	shell->env = copy_env(envp);
	if (!shell->env)
	{
		ft_putendl_fd("minishell: error: failed to copy environment", 2);
		exit(1);
	}
	//Initialisation de la liste chainee de l'environnement de export
	shell->export_list = init_env_list(envp);
	sort_list(&shell->export_list);
	
	// Initialisation de la liste chaînée d'environnement
	env_list = init_env_list(envp);
	if (!env_list)
	{
		ft_putendl_fd("minishell: error: failed to initialize environment list", 2);
		free_env(shell->env);
		exit(1);
	}
	shell->env_list = env_list;
	
	// Gestion de SHLVL selon le contexte
	if (envp && envp[0] && shlvl_exists_in_envp(envp))
	{
		// Nous sommes dans un sous-shell, incrémenter SHLVL
		if (increment_shlvl(env_list) != 0)
		{
			ft_putendl_fd("minishell: warning: failed to increment SHLVL", 2);
			// On continue malgré l'erreur - ce n'est pas critique
		}
	}
	else if (envp && envp[0])
	{
		// Nous sommes dans le shell racine, s'assurer que SHLVL=1
		if (ensure_shlvl_is_one(env_list) != 0)
		{
			ft_putendl_fd("minishell: warning: failed to set SHLVL", 2);
		}
	}

	// Maintenant, nous devons synchroniser env avec env_list car SHLVL a été modifié
	if (shell->env)
	{
		free_env(shell->env);					  // Libère l'ancienne copie
		shell->env = env_to_char_array(env_list); // Crée une nouvelle copie à jour
	}
}

/**
 * @brief Copie l'environnement système dans un tableau de chaînes alloué.
 *
 * @param envp L'environnement original (reçu dans main).
 * @return char** Une copie modifiable de l'environnement.
 */
char **copy_env(char **envp)
{
	int i;
	char **env;

	i = 0;
	// Compte le nombre d'éléments dans envp
	while (envp[i])
		i++;
	// Alloue de la mémoire pour le tableau et set à NULL
	env = ft_calloc((i + 1), sizeof(char *));
	if (!env)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		// Copie chaque élément de envp dans le nouveau tableau
		env[i] = strdup(envp[i]);
		if (!env[i])
		{
			free_env(env); // Libère la mémoire en cas d'erreur
			return (NULL);
		}
		i++;
	}
	return (env);
}
