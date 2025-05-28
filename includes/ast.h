/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 17:24:19 by hugoganet         #+#    #+#             */
/*   Updated: 2025/05/28 11:51:29 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_H
#define AST_H

#include "minishell.h"

// ! ----------------------- STRUCTURES --------------

/**
 * @enum e_ast_type
 * @brief Définit le type logique d’un nœud dans l’arbre AST.
 */
typedef enum e_ast_type
{
	AST_CMD,  /** Commande simple : exécutable + args */
	AST_PIPE, /** Opérateur pipe '|' */
	AST_AND,  /** Opérateur logique '&&' */
	AST_OR,	  /** Opérateur logique '||' */
	AST_GROUP /** Bloc entre parenthèses (...) */
}	t_ast_type;

/**
 * @struct s_ast
 * @brief Représente un nœud de l’arbre d’exécution logique.
 *		  Cette structure est utilisée pour construire l'AST (Abstract Syntax Tree)
 *
 * Elle contient :
 * 
 * - type : le type logique du nœud (commande, opérateur, etc.)
 * 
 * - cmd : un tableau de chaînes de caractères (argv) pour les commandes AST_CMD
 * 
 * - left : pointeur vers la branche gauche (ex: première commande dans un pipe)
 * 
 * - right : pointeur vers la branche droite (ex: deuxième commande dans un pipe)
 */
typedef struct s_ast
{
	t_ast_type		type; /** Type logique du nœud */
	char			**cmd; /** Tableau de chaînes de caractères pour les commandes */
	struct s_ast	*left; /** Pointeur vers la branche gauche */
	struct s_ast	*right; /** Pointeur vers la branche droite */
}					t_ast;

// ! ----------------------- FUNCTIONS ---------------

t_ast	*build_ast(t_token *tokens);
void	print_ast(t_ast *node, int depth);

#endif
