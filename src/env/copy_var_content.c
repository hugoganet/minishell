/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_var_content.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 01:34:50 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/01 11:22:39 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Vérifie si le nom correspond à un paramètre positionnel : $1, $9...
 *
 * Cette fonction vérifie si le nom de la variable est un chiffre unique,
 * ce qui indique qu'il s'agit d'un paramètre positionnel (par exemple, $1, $2, etc.).
 *
 * @param name Nom de la variable à vérifier.
 * @return `true` si c'est un paramètre positionnel, `false` sinon.
 */
bool is_positional_param(const char *name)
{
	return (ft_strlen(name) == 1 && ft_isdigit(name[0]));
}

/**
 * @brief Récupère la valeur d'une variable d'environnement (ex: USER -> elaudrez).
 *
 * @param name Nom de la variable (sans $).
 * @param env  Tableau des env (ex: USER=elaudrez).
 * @return Copie de la valeur ou NULL si absente.
 */
char *get_env_var_value(char *name, char **env)
{
	int i;
	int len;

	if (!name)
		return (NULL);
	len = ft_strlen(name);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], name, len) == 0 && env[i][len] == '=')
			return (ft_strdup(env[i] + len + 1));
		i++;
	}
	return (NULL);
}

/**
 * @brief Renvoie une copie brute de la variable dans la chaîne, sans expansion.
 *
 * Cette fonction est utilisée quand une variable n’est pas trouvée dans l’environnement :
 * elle permet de conserver littéralement `$VAR` ou `${VAR}` dans la chaîne finale.
 *
 * @param str   Chaîne source contenant l’appel `$VAR` ou `${VAR}`.
 * @param start Index de début du token `$`.
 * @param end   Index de fin après la variable.
 * @return Copie allouée du segment `$...`, ou NULL si erreur d’allocation.
 */
char *get_raw_token_if_invalid(char *str, int start, int end)
{
	if (!str || start >= end)
		return (NULL);
	return (ft_substr(str, start, end - start));
}

/**
 * @brief Gère les cas spéciaux de variables (paramètres positionnels, $ isolé, etc.).
 *
 * @param name_var Nom de la variable à vérifier.
 * @return Chaîne allouée pour les cas spéciaux, NULL sinon.
 */
static char *handle_special_cases(char *name_var)
{
	if (is_positional_param(name_var))
		return (ft_strdup(""));
	if (ft_strlen(name_var) == 0)
		return (ft_strdup(""));
	if (ft_strlen(name_var) == 1 && name_var[0] == '$')
		return (ft_strdup("$"));
	return (NULL);
}

/**
 * @brief Retourne la valeur d'une variable environnement si elle existe,
 * sinon retourne une chaîne vide pour imiter Bash.
 */
char *copy_var_content(char *str, t_shell *data, int *start, int *end)
{
	char *name_var;
	char *value;
	char *special_case;

	// Extraire le nom de la variable depuis la chaîne
	name_var = find_var(str, start, end);
	// Si le nom est invalide, retourner le token brut
	if (!name_var)
		return (NULL);
	special_case = handle_special_cases(name_var);
	if (special_case)
	{
		free(name_var);
		return (special_case);
	}
	value = get_env_var_value(name_var, data->env);
	if (!value)
	{
		free(name_var);
		return (ft_strdup(""));
	}
	free(name_var);
	return (value);
}
