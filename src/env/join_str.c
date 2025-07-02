/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_str.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 01:37:08 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/02 14:42:36 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Libère les anciennes chaînes utilisées dans join_str.
 *
 * Cette fonction libère la mémoire allouée pour les chaînes temporaires
 * utilisées lors de l'expansion de variables. Elle évite les fuites mémoire
 * en centralisant la libération de toutes les chaînes intermédiaires.
 *
 * @param prefix La partie avant la variable expansée
 * @param var La valeur de la variable (sera libérée)
 * @param suffix La partie après la variable expansée
 * @param tmp La chaîne temporaire résultant de prefix + var
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
 * Cette fonction découpe la chaîne originale autour de la variable `$VAR`,
 * puis recompose une nouvelle chaîne avec la valeur substituée.
 * Elle met à jour l'offset pour pointer après la valeur insérée.
 *
 * @param str La chaîne originale (sera libérée)
 * @param var La valeur de la variable à insérer
 * @param ctx La structure contenant les positions et l'offset
 * @return La chaîne reconstruite avec la variable remplacée
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
 * @brief Traite une seule occurrence de `$...` à partir d'un offset donné.
 *
 * Cette fonction gère le processus complet d'expansion d'une variable :
 * 1. Teste si c'est $? (statut de sortie)
 * 2. Teste si c'est une variable classique ($VAR, ${VAR}, $1...)
 * 3. Gère les cas spéciaux ($ isolé, etc.)
 * 4. Appelle l'expansion si une variable valide est trouvée
 *
 * @param str La chaîne à traiter
 * @param offset Pointeur vers l'offset actuel (sera mis à jour)
 * @param data Les données du shell (environnement, statut de sortie)
 * @return La chaîne mise à jour avec la variable expansée
 */
char *process_next_dollar(char *str, int *offset, t_shell *data)
{
	int start;
	int end;
	char *var;
	t_expand_ctx ctx;

	start = 0;
	end = 0;
	// Priorité 1 : tester si on est sur `$?`
	var = expand_exit_status(&str[*offset], data, &start, &end);
	// Gestion des variables classiques ($VAR, ${VAR}, $9...) et autres cas spéciaux.
	if (!var)
		var = copy_var_content(&str[*offset], data, &start, &end);
	// printf("process_next_dollar: var='%s'\n",var ? var : "NULL");
	// Prépare le contexte d'expansion
	ctx.start = start + *offset;
	ctx.end = end + *offset;
	ctx.offset = offset;
	return (handle_var_expansion(str, var, ctx));
}

/**
 * @brief Expansion complète d'une chaîne en remplaçant toutes les variables.
 *
 * Cette fonction parcourt la chaîne `str` et remplace chaque occurrence de `$VAR`
 * ou `${VAR}` par la valeur correspondante dans l'environnement.
 * Elle gère également les paramètres positionnels et l'expansion du statut de sortie `$?`.
 * L'expansion respecte les règles des quotes : pas d'expansion dans les quotes simples.
 *
 * @param str La chaîne d'entrée contenant des variables à expanser
 * @param data Les données du shell contenant l'environnement et le statut de sortie
 * @return La chaîne modifiée avec les variables expansées
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
