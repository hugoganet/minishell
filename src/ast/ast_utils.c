/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 12:04:03 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/09 12:05:58 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Compte le nombre d'arguments ARG après un nœud CMD.
 *
 * @param node Le nœud CMD de départ
 * @return Le nombre d'arguments trouvés
 */
int count_args(t_token *node)
{
	int count;
	t_token *ptr;

	count = 0;
	ptr = node->next;
	while (ptr && ptr->type != CMD && ptr->type != PIPE)
	{
		if (ptr->type == ARG)
			count++;
		ptr = ptr->next;
	}
	return (count);
}


/**
 * @brief Copie un argument dans le tableau d'arguments de l'AST.
 *
 * Cette fonction duplique la chaîne de caractères du token courant (ptr->str)
 * et la place dans le tableau new_ast->args à l'index *j. Elle protège l'allocation
 * mémoire : en cas d'échec de ft_strdup, elle libère les arguments déjà alloués
 * et signale l'erreur.
 *
 * @param ptr Pointeur vers le token courant (doit contenir le champ str)
 * @param new_ast Pointeur vers la structure AST où stocker l'argument
 * @param j Pointeur vers l'index courant dans le tableau d'arguments (incrémenté si succès)
 * @return 0 en cas de succès, 1 en cas d'erreur d'allocation mémoire
 */
int is_arg(t_token *ptr, t_ast *new_ast, int *j)
{
	new_ast->args[*j] = ft_strdup(ptr->str);
	if (!new_ast->args[*j])
	{
		free_args_on_error(new_ast->args, *j);
		*new_ast->args = NULL;
		return (1);
	}
	(*j)++;
	return (0);
}