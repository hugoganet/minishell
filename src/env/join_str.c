/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_str.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 01:37:08 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/01 09:59:52 by hugoganet        ###   ########.fr       */
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
 * @brief Remplace une variable par sa valeur dans la chaîne source.
 *
 * Cette fonction découpe la chaîne `str` en trois parties :
 *   - le préfixe avant `$`
 *   - la valeur `var` à insérer
 *   - le suffixe après la variable
 *
 * Elle reconstruit la chaîne finale en concaténant les trois morceaux,
 * libère l’ancienne chaîne, et met à jour `*offset`.
 *
 * @param str Chaîne d’entrée (sera libérée).
 * @param var Valeur à insérer à la place de la variable.
 * @param ctx Contexte contenant `start`, `end`, et `offset`.
 * @return Chaîne finale reconstruite (à libérer plus tard).
 */
char *handle_var_expansion(char *str, char *var, t_expand_ctx ctx)
{
	char *prefix;
	char *suffix;
	char *tmp;
	char *final;

	prefix = ft_substr(str, 0, ctx.start);
	suffix = ft_substr(str, ctx.end, ft_strlen(str) - ctx.end);
	tmp = ft_strjoin(prefix, var);
	final = ft_strjoin(tmp, suffix);
	*(ctx.offset) = ctx.start + ft_strlen(var);
	free_temp(prefix, var, suffix, tmp);
	free(str);
	return (final);
}

static char *fill_ctx_and_expand(char *str, char *var, int start, int end, int *offset)
{
	t_expand_ctx ctx;

	ctx.start = start + *offset;
	ctx.end = end + *offset;
	ctx.offset = offset;
	return (handle_var_expansion(str, var, ctx));
}

/**
 * @brief Traite une seule occurrence de `$...` à partir d’un offset donné.
 */
char *process_next_dollar(char *str, int *offset, t_shell *data)
{
	int start;
	int end;
	char *var;

	start = 0;
	end = 0;
	// Priorité 1 : tester si on est sur `$?`
	var = expand_exit_status(&str[*offset], data, &start, &end);
	// Priorité 2 : variable classique ($VAR, ${VAR}, $9...)
	if (!var)
		var = copy_var_content(&str[*offset], data, &start, &end);
	// Aucun remplacement → avancer d’un caractère et continuer
	if (!var)
	{
		(*offset)++;
		return (str);
	}
	// Cas spécial : on a juste un `$` non suivi de nom valide → ne pas remplacer
	if (ft_strlen(var) == 1 && var[0] == '$')
	{
		free(var);
		*offset += end;
		return (str);
	}
	return (fill_ctx_and_expand(str, var, start, end, offset));
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
	char *dollar_pos;

	offset = 0;
	while ((dollar_pos = ft_strchr(&str[offset], '$')) != NULL)
	{
		offset = dollar_pos - str;
		str = process_next_dollar(str, &offset, data);
	}
	return (str);
}
