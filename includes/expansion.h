/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 17:36:22 by elaudrez          #+#    #+#             */
/*   Updated: 2025/07/07 21:40:16 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANSION_H
# define EXPANSION_H

# include "minishell.h"
# include <stdbool.h>

/**
 * @brief Structure pour gérer l'état de l'expansion.
 * 	  Contient la chaîne d'entrée, la chaîne de sortie,
 *    l'index de lecture, et les états des quotes.
 *    Cette structure est utilisée pour suivre l'état de l'expansion
 *    des variables dans la chaîne d'entrée.
 *
 * @param input La chaîne d'entrée à traiter.
 * @param output La chaîne de sortie après expansion.
 * @param i L'index de lecture dans la chaîne d'entrée.
 * @param in_single_quotes Flag pour l'état des quotes simples.
 * @param in_double_quotes Flag pour l'état des quotes doubles.
 * @param expanded_to_empty Flag pour indiquer si une variable s'est
 *							 expansée en chaîne vide.
 */
typedef struct s_expansion_state
{
	const char	*input;
	char		*output;
	int			i;
	bool		in_single_quotes;
	bool		in_double_quotes;
	bool		expanded_to_empty;
}	t_expansion_state;

// !===========================================================================
// !                             EXPANSION.C                                 =
// !===========================================================================

/**
 * @brief Point d'entrée principal pour l'expansion des variables.
 *        Orchestre l'analyse de la chaîne et la substitution des variables.
 *
 * @param input La chaîne de caractères à traiter.
 * @param env_list La liste des variables d'environnement.
 * @param last_exit_status Le code de retour de la dernière commande.
 * @return Une nouvelle chaîne allouée avec les variables expansées.
 */
char	*expand_variables(const char *input, t_env *env_list,
			int last_exit_status);

/**
 * @brief Version de expand_variables qui retourne aussi si l'expansion a
 *		  résulté en chaîne vide.
 *
 * @param input La chaîne de caractères à traiter.
 * @param env_list La liste des variables d'environnement.
 * @param last_exit_status Le code de retour de la dernière commande.
 * @param expanded_to_empty Pointeur vers un booléen qui sera mis à true si
 *								 l'expansion donne une chaîne vide.
 * @return Une nouvelle chaîne allouée avec les variables expansées.
 */
char	*expand_variables_with_flag(const char *input, t_env *env_list,
			int last_exit_status, bool *expanded_to_empty);

// !===========================================================================
// !                         EXPANSION_QUOTES.C                              =
// !===========================================================================

/**
 * @brief Met à jour l'état des quotes (simples/doubles) en fonction du
 *        caractère courant.
 *
 * @param state L'état actuel de l'expansion.
 */
void	expansion_update_quote_state(t_expansion_state *state);

// !===========================================================================
// !                        EXPANSION_SPECIALS.C                             =
// !===========================================================================

/**
 * @brief Gère l'expansion d'une variable spéciale (ex: $?, $$).
 *
 * @param state L'état actuel de l'expansion.
 * @param last_exit_status Le code de retour de la dernière commande.
 * @return Le contenu de la variable spéciale, ou NULL si non applicable.
 */
char	*expand_special_variable(t_expansion_state *state,
			int last_exit_status);
bool	try_expand_special_vars(t_expansion_state *state, int last_exit_status);

// !===========================================================================
// !                         EXPANSION_UTILS.C                               =
// !===========================================================================
/**
 * @brief Extrait le nom d'une variable à partir de la position actuelle.
 *        Ex: pour "$USER_VAR", extrait "USER_VAR".
 *
 * @param input La chaîne d'entrée.
 * @param start_index L'index de début (après le '$').
 * @return Le nom de la variable (chaîne allouée).
 */
char *extract_variable_name(const char *input, int start_index);

/**
 * @brief Recherche la valeur d'une variable dans l'environnement.
 *
 * @param var_name Le nom de la variable à rechercher.
 * @param env_list L'environnement sous forme de liste chaînée.
 * @return La valeur de la variable (chaîne allouée), ou NULL si non trouvée.
 */
char	*get_env_variable(const char *var_name, t_env *env_list);

/**
 * @brief Traverse l'AST et expanse les variables dans les arguments des
 *		  commandes.
 *
 * @param node Le nœud racine de l'AST à traiter.
 * @param shell La structure principale du shell contenant l'environnement
 *				  et le statut.
 */
void	expand_ast_arguments(t_ast *node, t_shell *shell);

// !===========================================================================
// !                      EXPANSION_UTILS2.C                                 =
// !===========================================================================

bool	is_translated_string(const char *input, int dollar_pos);
char	*extract_translated_string(const char *input, int dollar_pos,
			int *end_pos);
void	append_to_output(t_expansion_state *state, const char *to_add);
void	append_and_free(t_expansion_state *state, char *value);
int		init_expansion_state(t_expansion_state *state, const char *input);

#endif
