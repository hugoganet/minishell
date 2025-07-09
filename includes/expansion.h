/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 17:36:22 by elaudrez          #+#    #+#             */
/*   Updated: 2025/07/09 13:43:22 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANSION_H
# define EXPANSION_H

# include "minishell.h"
# include <stdbool.h>

/**
 * @struct s_expansion_state
 * @brief Structure pour gérer l'état de l'expansion des variables.
 * 
 * Cette structure maintient l'état durant le processus d'expansion des variables
 * dans une chaîne de caractères. Elle suit la position actuelle dans la chaîne
 * d'entrée, accumule le résultat dans la chaîne de sortie, et maintient l'état
 * des guillemets pour déterminer si l'expansion doit avoir lieu. Elle permet
 * également de détecter si une expansion a résulté en une chaîne vide.
 *
 * @param input La chaîne d'entrée à traiter pour l'expansion
 * @param output La chaîne de sortie accumulée après expansion
 * @param i L'index de lecture actuel dans la chaîne d'entrée
 * @param in_single_quotes Flag indiquant si on est à l'intérieur de
 *                        guillemets simples
 * @param in_double_quotes Flag indiquant si on est à l'intérieur de
 *                        guillemets doubles
 * @param expanded_to_empty Flag indiquant si une variable s'est expansée
 *                         en chaîne vide
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

char	*expand_variables(const char *input, t_env *env_list,
			int last_exit_status);

char	*expand_variables_with_flag(const char *input, t_env *env_list,
			int last_exit_status, bool *expanded_to_empty);

// !===========================================================================
// !                         EXPANSION_QUOTES.C                              =
// !===========================================================================

void	expansion_update_quote_state(t_expansion_state *state);

// !===========================================================================
// !                        EXPANSION_SPECIALS.C                             =
// !===========================================================================

char	*expand_special_variable(t_expansion_state *state,
			int last_exit_status);
bool	try_expand_special_vars(t_expansion_state *state, int last_exit_status);

// !===========================================================================
// !                         EXPANSION_UTILS.C                               =
// !===========================================================================

char	*extract_variable_name(const char *input, int start_index);
char	*get_env_variable(const char *var_name, t_env *env_list);
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
