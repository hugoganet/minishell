/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_execution.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 12:44:43 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/07 11:50:55 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"
#include "pipe.h"
#include <stdbool.h>

/**
 * @brief Exécute un nœud de type PIPE de l'AST.
 *
 * Détecte si c'est un pipeline simple (2 commandes) ou complexe (plus de 2),
 * puis utilise la logique appropriée pour l'exécution.
 *
 * @param node Le nœud de type PIPE
 * @param env Liste chaînée des variables d'environnement
 * @param shell Structure principale du shell
 * @return Code de retour de la dernière commande du pipeline
 */
int execute_pipe_node(t_ast *node, t_env *env, t_shell *shell)
{
	// Si c'est un pipeline complexe (plus de 2 commandes)
	if (is_complex_pipeline(node))
		return (execute_complex_pipeline(node, env, shell));

	// Sinon, utiliser l'ancienne logique pour les pipes simples
	return (execute_simple_pipe(node, env, shell));
}
