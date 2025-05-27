/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 17:24:19 by hugoganet         #+#    #+#             */
/*   Updated: 2025/05/27 17:26:05 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_H
#define AST_H

#include "minishell.h"

/**
 * @enum e_ast_type
 * @brief Définit le type logique d’un nœud dans l’arbre AST.
 */
typedef enum e_ast_type
{
	AST_CMD,  /**< Commande simple : exécutable + args */
	AST_PIPE, /**< Opérateur pipe '|' */
	AST_AND,  /**< Opérateur logique '&&' */
	AST_OR,	  /**< Opérateur logique '||' */
	AST_GROUP /**< Bloc entre parenthèses (...) */
}	t_ast_type;

/**
 * @struct s_ast
 * @brief Représente un nœud de l’arbre d’exécution logique.
 *		  Cette structure est utilisée pour construire l'AST (Abstract Syntax Tree)
 *
 * @var s_ast::type
 * Type du nœud (commande, opérateur, etc.)
 * @var s_ast::cmd
 * Tableau de chaînes (argv) pour les commandes AST_CMD uniquement.
 * @var s_ast::left
 * Branche gauche (ex: première commande dans un pipe)
 * @var s_ast::right
 * Branche droite (ex: deuxième commande dans un pipe)
 */
typedef struct s_ast
{
	t_ast_type		type;
	char			**cmd;
	struct s_ast	*left;
	struct s_ast	*right;
}					t_ast;

#endif
