/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 13:06:24 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/09 14:18:51 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"
#include "minishell.h"

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

	// On traite les variables régulières.
	// - $VAR
	// - ${VAR}
	// On ne gère pas les paramètres positionnels ($1, $2, etc.)
	var_name = extract_variable_name(state->input, state->i + 1);
	if (var_name && *var_name)
	{
		// On récupère la valeur de la variable dans l'environnement.
		var_value = get_env_variable(var_name, env_list);
		if (!var_value)
			state->expanded_to_empty = true;
		// On ajoute la valeur de la variable à state->output et on free var_value.
		append_and_free(state, var_value);
		if (state->input[state->i + 1] == '{')
			state->i += ft_strlen(var_name) + 3;
		else
			state->i += ft_strlen(var_name) + 1;
		free(var_name);
	}
	// Si var_name est NULL ou vide, on ne fait rien.
	// On retourne juste le caractère '$' dans state->output.
	else
	{
		free(var_name);
		// On ajoute le '$' à state->output.
		append_to_output(state, "$");
		// On incrémente l'index pour passer au caractère suivant.
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
	// On essaye d'abord d'expanser les variables spéciales.
	// - '$0' : nom du programme.
	// - '$?' : code de retour de la dernière commande.
	// - '$"..." : translated string. (non supporté)
	if (try_expand_special_vars(state, last_exit_status))
		return ;
	// Si ce n'est pas une variable spéciale,
	// on traite les variables régulières.
	expand_regular_variable(state, env_list);
}

/**
 * @brief Traite un caractère de la chaîne d'entrée.
 * Si c'est une quote, on met à jour l'état des quotes.
 * Si c'est un '$', on gère l'expansion des variables.
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

	// c est le caractère actuel de l'entrée.
	c = state->input[state->i];
	// Si c'est une quote, et que l'on n'est pas dans une quote,
	// on met à jour l'état des quotes.
	if ((c == '\'' && !state->in_double_quotes)
		|| (c == '"' && !state->in_single_quotes))
	{
		expansion_update_quote_state(state);
		state->i++;
	}
	// Si c est un '$' et que l'on n'est pas dans des quotes simples,
	// on récupère le nom de la variable et on l'expanse.
	else if (c == '$' && !state->in_single_quotes)
		handle_dollar(state, env_list, last_exit_status);
	else
	{
		// Si c n'est pas un caractère spécial, on l'ajoute à state->output.
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

	// On initialise l'état d'expansion (structure t_expansion_state).
	// A ce stade, state->input est l'input donné.
	// state->output est alloué à une chaîne vide.
	if (!init_expansion_state(&state, input))
		return (NULL);
	// On traite la chaîne d'entrée caractère par caractère.
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

	// On init la structure d'état d'expansion.
	// A ce stade, state->input est l'input donné.
	// state->output est alloué à une chaîne vide. 
	if (!init_expansion_state(&state, input))
		return (NULL);
	// On traite la chaîne d'entrée caractère par caractère.
	while (state.input[state.i])
		handle_char(&state, env_list, last_exit_status);
	// ON vérifie si l'expansion à donné une chaîne vide.
	// ex : 
	if (state.expanded_to_empty && ft_strlen(state.output) == 0)
		*expanded_to_empty = true;
	else
		*expanded_to_empty = false;
	return (state.output);
}
