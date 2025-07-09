/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 16:08:41 by elaudrez          #+#    #+#             */
/*   Updated: 2025/07/09 18:09:24 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"
#include "libft.h"
#include <stdlib.h>

/**
 * @brief Extrait le nom d'une variable à partir de la position actuelle.
 *        Ex: pour "$USER_VAR", extrait "USER_VAR".
 * Gère aussi la syntaxe ${VAR}.
 * Rejette les paramètres positionnels ($0, $1, etc.) sauf $0.
 *
 * @param input La chaîne d'entrée.
 * @param start_index L'index de début (après le '$').
 * @return Le nom de la variable (chaîne allouée), ou NULL si invalide.
 */
char	*extract_variable_name(const char *input, int start_index)
{
	int	len;

	len = 0;
	if (!input[start_index])
		return (NULL);
	if (input[start_index] == '{')
	{
		start_index++;
		while (input[start_index + len] && input[start_index + len] != '}')
			len++;
		return (ft_substr(input, start_index, len));
	}
	if (ft_isdigit(input[start_index]))
		return (ft_substr(input, start_index, 1));
	if (!is_valid_var_start(input[start_index]))
		return (NULL);
	len++;
	while (is_valid_var_char(input[start_index + len]))
		len++;
	return (ft_substr(input, start_index, len));
}

/**
 * @brief Recherche la valeur d'une variable dans l'environnement.
 *        Les paramètres positionnels ($1, $2, etc.) retournent NULL
 * (vides). Seul $0 a une valeur (nom du shell, mais déjà géré par
 * l'expansion spéciale).
 *
 * @param var_name Le nom de la variable à rechercher.
 * @param env_list L'environnement sous forme de liste chaînée.
 * @return La valeur de la variable (chaîne allouée), ou NULL si
 * non trouvée/vide.
 */
char	*get_env_variable(const char *var_name, t_env *env_list)
{
	t_env	*current;

	if (ft_strlen(var_name) == 1 && ft_isdigit(var_name[0]))
		return (NULL);
	current = env_list;
	while (current)
	{
		if (ft_strcmp(current->key, var_name) == 0)
			return (ft_strdup(current->value));
		current = current->next;
	}
	return (NULL);
}

/**
 * @brief Extrait le contenu d'une translated string $"...".
 *        Par exemple, pour $"USER", retourne "USER" sans les guillemets.
 *
 * @param input La chaîne d'entrée.
 * @param dollar_pos La position du caractère '$'.
 * @param end_pos Pointeur pour stocker la position de fin (après le
 * guillemet fermant).
 * @return Le contenu de la translated string (chaîne allouée), ou
 * NULL si invalide.
 */
char	*extract_translated_string(
		const char *input, int dollar_pos, int *end_pos)
{
	int	start;
	int	len;
	int	i;

	if (!is_translated_string(input, dollar_pos))
		return (NULL);
	start = dollar_pos + 2;
	i = start;
	while (input[i] && input[i] != '"')
		i++;
	len = i - start;
	*end_pos = i + 1;
	return (ft_substr(input, start, len));
}

/**
 * @brief Ajoute une chaîne à la fin de la chaîne de sortie de l'état.
 *        Gère la réallocation et la libération de la mémoire.
 * @param state L'état actuel de l'expansion.
 * @param to_add La chaîne à ajouter.
 */
void	append_to_output(t_expansion_state *state, const char *to_add)
{
	char	*temp;

	if (!to_add)
		return ;
	temp = ft_strjoin(state->output, to_add);
	free(state->output);
	state->output = temp;
}

/**
 * @brief Ajoute une chaîne de caractères à la sortie de l'expansion
 * et la libère.
 *
 * @param state L'état de l'expansion.
 * @param value La chaîne à ajouter (sera libérée par la fonction).
 */
void	append_and_free(t_expansion_state *state, char *value)
{
	if (!value)
		return ;
	append_to_output(state, value);
	free(value);
}
