/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_execution.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 12:44:43 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/08 18:58:07 by hugoganet        ###   ########.fr       */
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
int	execute_pipe_node(t_ast *node, t_env *env, t_shell *shell)
{
	// On compte le nombre total de COMMANDES dans l'AST.
	if (is_complex_pipeline(node))
		return (execute_complex_pipeline(node, env, shell));
	// Si c'est un pipeline simple (2 commandes), on utilise l'exécution optimisée.
	// Cette fonction est optimisée pour les pipelines simples à 2 processus.
	return (execute_simple_pipe(node, env, shell));
}

/**
 *
 * ? ok donc en fait, pour déterminer la valeur de is_complex_pipeline, on
 * ? part forcément d'un noeud PIPE (détecter dans @src/ast/ast_executor.c)
 * ? et à partir de ce noeud, on compte toute les commandes qui suivent,
 * ? donc même si la commande est ` ls | CMD >file CMD <file | CMD <<file |
 * ? CMD >>file`, on executera 'execute_complex_pipeline()` car c'est en
 * ? fonction du nombre de CMD TOTAL de l'AST à partir du moment ou il y'a
 * ? un pipe ?
 *
 * OUI, vous avez parfaitement compris !
 * 
 *  1. Détection du nœud PIPE

  Dans ast_executor.c ligne 30-31 :
  if (node->type == PIPE)
	  return (execute_pipe_node(node, env_list, shell));

  2. Comptage de TOUTES les commandes du pipeline

  Dans execute_pipe_node(), on appelle is_complex_pipeline() qui utilise
   count_pipeline_commands() pour compter toutes les commandes dans
  l'arbre PIPE entier.

  3. Votre exemple est parfait :

  ls | CMD >file CMD <file | CMD <<file | CMD >>file

  Même si certaines commandes ont des redirections complexes,
  count_pipeline_commands() va compter :
  - 1 commande pour ls
  - 1 commande pour CMD >file (nœud REDIR_OUTPUT contenant une CMD)
  - 1 commande pour CMD <file (nœud REDIR_INPUT contenant une CMD)
  - 1 commande pour CMD <<file (nœud HEREDOC contenant une CMD)
  - 1 commande pour CMD >>file (nœud REDIR_APPEND contenant une CMD)

  Total = 5 commandes > 2 → execute_complex_pipeline()

  4. La logique de décision :

  - ≤ 2 commandes → execute_simple_pipe() (optimisé pour 2 processus)
  - > 2 commandes → execute_complex_pipeline() (générique pour N
  processus)

  C'est pourquoi le dernier if avec find_cmd_node() est crucial : il
  permet de détecter les commandes même quand elles sont "cachées" dans
  des nœuds de redirection, pour avoir le bon décompte total et choisir
  la bonne stratégie d'exécution.
 */

/**
 * ? Du coup il ne compte que les commande après le pipe ? Pas celle avant ?

⏺ NON, il compte TOUTES les commandes du pipeline entier !

 Regardez bien la fonction count_pipeline_commands() dans pipe_utils.c
 :

 if (node->type == PIPE)
	 return (count_pipeline_commands(node->left)
			 + count_pipeline_commands(node->right));

 Cette récursion parcourt tout l'arbre PIPE :
 - node->left = partie gauche du pipe
 - node->right = partie droite du pipe

 Exemple concret :

 Pour cmd1 | cmd2 | cmd3, l'AST ressemble à :
		 PIPE
		/    \
	 cmd1   PIPE
		   /    \
		cmd2   cmd3

 Quand count_pipeline_commands() est appelée sur le nœud PIPE racine :
 1. Elle compte cmd1 (gauche) = 1
 2. Elle compte le sous-arbre droit (PIPE) qui contient cmd2 et cmd3 =
 2
 3. Total = 1 + 2 = 3 commandes

 Pour votre exemple :

 ls | grep txt | wc -l

 La fonction compte les 3 commandes (ls, grep, wc) même si on part du
 nœud PIPE racine.

 C'est exactement pourquoi la logique fonctionne : elle compte toutes
 les commandes du pipeline complet, pas seulement celles après le pipe
 !
 */
