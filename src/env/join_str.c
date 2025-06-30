/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_str.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 01:37:08 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/01 08:17:04 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Libère les anciennes chaînes utilisées dans join_str.
 */
static void free_temp(char *prefix, char *var, char *suffix, char *tmp)
{
	free(prefix);
	free(var);
	free(suffix);
	free(tmp);
}

/**
 * @brief Gère une seule expansion de `$VAR`, en reconstruisant la chaîne.
 */
char *handle_var_expansion(char *str, char *var, int start, int end, int *offset)
{
	char *prefix;
	char *suffix;
	char *tmp;
	char *final;

	prefix = ft_substr(str, 0, start);
	suffix = ft_substr(str, end, ft_strlen(str) - end);
	tmp = ft_strjoin(prefix, var);
	final = ft_strjoin(tmp, suffix);
	*offset = start + ft_strlen(var);
	free_temp(prefix, var, suffix, tmp);
	free(str);
	return (final);
}

/**
 * @brief Traite un seul `$...` dans la chaîne à partir de offset, et reconstruit `str`.
 * Cette fonction gère l'expansion de variables d'environnement,
 * les paramètres positionnels, et l'expansion du statut de sortie `$?`.
 * Elle met à jour `offset` pour pointer après la variable traitée.
 * 
 * @param str    La chaîne d'entrée contenant des variables à expanser.
 * @param offset Pointeur vers l'offset actuel dans la chaîne.
 * @param data   Les données du shell contenant l'environnement et le statut de sortie.
 * @return La chaîne modifiée avec la variable expansée, ou la chaîne originale si aucune variable n'est trouvée.
 */
char *process_next_dollar(char *str, int *offset, t_shell *data)
{
	int start;
	int end;
	char *var;

	start = 0;
	end = 0;
	var = expand_exit_status(&str[*offset], data, &start, &end);
	if (!var)
		var = copy_var_content(&str[*offset], data, &start, &end);
	if (!var)
	{
		(*offset)++;
		return (str);
	}
	start += *offset;
	end += *offset;
	if (ft_strlen(var) == 1 && var[0] == '$')
	{
		free(var);
		*offset = end;
		return (str);
	}
	return (handle_var_expansion(str, var, start, end, offset));
}

/**
 * @brief Expansion complète de la chaîne en remplaçant toutes les variables d'environnement.
 * Cette fonction parcourt la chaîne `str` et remplace chaque occurrence de `$VAR`
 * ou `${VAR}` par la valeur correspondante dans l'environnement.
 * Elle gère également les paramètres positionnels et l'expansion du statut de sortie `$?`.
 * 
 * @param str La chaîne d'entrée contenant des variables à expanser.
 * @param data Les données du shell contenant l'environnement et le statut de sortie.
 * @return La chaîne modifiée avec les variables expansées, ou la chaîne originale si aucune variable n'est trouvée.
 */
char *join_str(char *str, t_shell *data)
{
	int offset;

	offset = 0;
	while (ft_strchr(&str[offset], '$'))
		str = process_next_dollar(str, &offset, data);
	return (str);
}
