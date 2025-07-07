/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 13:06:24 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/07 16:18:40 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"
#include "minishell.h"

/**
 * @brief Initialise l'état de l'expansion.
 * 		  Alloue de la mémoire pour la chaîne de sortie et
 * initialise les variables
 * 		  d'état.
 *
 * @param state L'état d'expansion à initialiser.
 * @param input La chaîne d'entrée à traiter.
 * @return `1` en cas de succès, `0` en cas d'échec
 */
int	init_expansion_state(t_expansion_state *state, const char *input)
{
	state->input = input;
	state->output = ft_strdup("");
	if (!state->output)
		return (0);
	state->i = 0;
	state->in_single_quotes = false;
	state->in_double_quotes = false;
	state->expanded_to_empty = false;
	return (1);
}

/**
 * @brief Essaie d'expander les variables spéciales ou les translated strings.
 *		  Gère les variables spéciales comme $?, $0, $"".
 *
 * @param state L'état actuel de l'expansion.
 * @param last_exit_status Le code de retour de la dernière commande.
 * @return `true` si une variable spéciale a été trouvée et expansée,
 * false sinon.
 */
static bool	try_expand_special_vars(t_expansion_state *state,
									int last_exit_status)
{
	char	*var_value;
	int		end_pos;

	var_value = extract_translated_string(state->input, state->i, &end_pos);
	if (var_value)
	{
		append_and_free(state, var_value);
		state->i = end_pos;
		return (true);
	}
	var_value = expand_special_variable(state, last_exit_status);
	if (var_value)
	{
		append_and_free(state, var_value);
		return (true);
	}
	return (false);
}

/**
 * @brief Extrait le nom d'une variable régulière à partir de la
 * position actuelle.
 *        Gère la syntaxe ${VAR} ou $VAR.
 *
 * @param state L'état actuel de l'expansion.
 * @param env_list La liste des variables d'environnement.
 */
static void	expand_regular_variable(t_expansion_state *state, t_env *env_list)
{
	char	*var_name;
	char	*var_value;

	var_name = extract_variable_name(state->input, state->i + 1);
	if (var_name && *var_name)
	{
		var_value = get_env_variable(var_name, env_list);
		if (!var_value)
		{
			// Variable non trouvée, marquer que l'expansion a donné une chaîne vide
			state->expanded_to_empty = true;
		}
		// Si on a une variable valide, on l'expanse et l'ajoute à state->output
		append_and_free(state, var_value);
		// On saute le nom de la var + le '$' et '{}'
		if (state->input[state->i + 1] == '{')
			state->i += ft_strlen(var_name) + 3;
		// On saute juste le nom de la var + le '$' si pas de '{}'
		else
			state->i += ft_strlen(var_name) + 1;
		free(var_name);
	}
	else
	{
		// Si on n'a pas de variable valide, on ajoute juste le '$' à l'output
		// et on avance l'index de 1
		// Ex : pour "$USER_VAR", si USER_VAR n'existe pas
		free(var_name);
		append_to_output(state, "$");
		state->i++;
	}
}

/**
 * @brief Fonction d'orchestration pour traiter le caractère '$'.
 * 	   	  Gère les variables spéciales et les variables régulières.
 *
 * @param state L'état actuel de l'expansion.
 * @param env_list La liste des variables d'environnement.
 * @param last_exit_status Le code de retour de la dernière commande.
 */
static void	handle_dollar(t_expansion_state *state, t_env *env_list,
			int last_exit_status)
{
	if (try_expand_special_vars(state, last_exit_status))
		return ;
	expand_regular_variable(state, env_list);
}

/**
 * @brief Traite un caractère de la chaîne d'entrée.
 *
 * @param state L'état actuel de l'expansion.
 * @param env_list L'environnement.
 * @param last_exit_status Le code de retour de la dernière commande.
 */
static void	handle_char(t_expansion_state *state, t_env *env_list,
						int last_exit_status)
{
	char	c;
	char	char_to_add[2];

	c = state->input[state->i];
	if ((c == '\'' && !state->in_double_quotes)
		|| (c == '"' && !state->in_single_quotes))
	{
		expansion_update_quote_state(state);
		state->i++;
	}
	else if (c == '$' && !state->in_single_quotes)
		handle_dollar(state, env_list, last_exit_status);
	else
	{
		// Si le caractère n'est pas une quote ou un dollar,
		// on l'ajoute directement à state->output et on termine avec un '\0'
		char_to_add[0] = c;
		char_to_add[1] = '\0';
		append_to_output(state, char_to_add);
		state->i++;
	}
}

/**
 * @brief Point d'entrée principal pour l'expansion des variables.
 *        Orchestre l'analyse de la chaîne et la substitution des variables.
 *
 * @param input La chaîne de caractères à traiter.
 * @param env_list La liste des variables d'environnement.
 * @param last_exit_status Le code de retour de la dernière commande.
 * @return `char *` Une nouvelle chaîne allouée avec les variables expansées.
 */
char	*expand_variables(const char *input, t_env *env_list,
		int last_exit_status)
{
	t_expansion_state	state;

	if (!init_expansion_state(&state, input))
		return (NULL);
	// On parcourt la chaîne d'entrée caractère par caractère
	while (state.input[state.i])
		handle_char(&state, env_list, last_exit_status);
	return (state.output);
}

/**
 * @brief Version de expand_variables qui retourne aussi si
 * l'expansion a résulté en chaîne vide.
 *
 * @param input La chaîne de caractères à traiter.
 * @param env_list La liste des variables d'environnement.
 * @param last_exit_status Le code de retour de la dernière commande.
 * @param expanded_to_empty Pointeur vers un booléen qui sera mis à true
 * si l'expansion donne une chaîne vide.
 * @return Une nouvelle chaîne allouée avec les variables expansées.
 */
char	*expand_variables_with_flag(const char *input, t_env *env_list,
	int last_exit_status, bool *expanded_to_empty)
{
	t_expansion_state	state;

	if (!init_expansion_state(&state, input))
		return (NULL);
	// On parcourt la chaîne d'entrée caractère par caractère
	while (state.input[state.i])
		handle_char(&state, env_list, last_exit_status);
	// Si la chaîne d'entrée contenait une variable et que le résultat est vide
	if (state.expanded_to_empty && ft_strlen(state.output) == 0)
		*expanded_to_empty = true;
	else
		*expanded_to_empty = false;
	return (state.output);
}
