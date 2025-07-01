/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_str.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 01:37:08 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/01 11:12:42 by hugoganet        ###   ########.fr       */
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
 * @brief Remplace une variable d'environnement par sa valeur.
 *
 * Cette fonction découpe la chaîne originale autour de `$`,
 * puis recompose une nouvelle chaîne avec la valeur substituée.
 *
 * @param str Chaîne originale (sera libérée).
 * @param var Valeur de la variable à insérer.
 * @param ctx Structure contenant les positions et offset.
 * @return Chaîne reconstruite avec la variable remplacée.
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

/**
 * @brief Détermine si le `$` à la position donnée doit être expansé.
 *
 * L'expansion est interdite à l'intérieur de quotes simples.
 *
 * @param str La chaîne d'origine.
 * @param pos L’index du `$` dans la chaîne.
 * @return 1 si l’expansion est autorisée, 0 sinon.
 */
static int should_expand_at_position(char *str, int pos)
{
	bool in_single;
	bool in_double;
	int i;

	in_single = false;
	in_double = false;
	i = 0;
	// On parcourt la chaîne jusqu'à la position donnée du `$`
	while (i < pos && str[i])
	{
		// On bascule l'état des quotes simples et doubles
		if (str[i] == '\'' && !in_double)
			in_single = !in_single;
		else if (str[i] == '"' && !in_single)
			in_double = !in_double;
		i++;
	}
	// Dans les quotes simples, pas d'expansion
	return (!in_single);
}

/**
 * @brief Prépare le contexte d’expansion et appelle handle_var_expansion.
 *
 * @param str La chaîne originale.
 * @param var La valeur expansée.
 * @param start Index du début de la variable.
 * @param end Index de fin de la variable.
 * @param offset Offset actuel, sera mis à jour.
 * @return Chaîne mise à jour.
 */
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
	// On commence par chercher le premier `$`
	dollar_pos = ft_strchr(&str[offset], '$');
	while (dollar_pos != NULL)
	{
		// On calcule l'offset de ce `$` par rapport à la chaîne originale
		offset = dollar_pos - str;
		// Si on est dans une position où l'expansion est autorisée
		// on appelle process_next_dollar pour traiter ce `$`
		if (should_expand_at_position(str, offset))
			str = process_next_dollar(str, &offset, data);
		// Sinon on avance d'un caractère
		else
			offset++;
		// On cherche le prochain `$` à partir de l'offset mis à jour
		dollar_pos = ft_strchr(&str[offset], '$');
	}
	return (str);
}
