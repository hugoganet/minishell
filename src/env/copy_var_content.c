/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_var_content.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 01:34:50 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/02 16:47:41 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Récupère la valeur d'une variable d'environnement.
 *
 * Cette fonction recherche une variable dans le tableau d'environnement
 * et retourne une copie de sa valeur si elle existe.
 * Format attendu dans env : "NOM=valeur"
 *
 * @param name Le nom de la variable (sans $)
 * @param env Le tableau des variables d'environnement
 * @return Une copie allouée de la valeur, ou NULL si la variable n'existe pas
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
		// Vérifie si le nom correspond ET qu'il y a un '=' après
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
 * @brief Retourne la valeur d'une variable d'environnement ou une chaîne vide.
 *
 * Cette fonction centralise la logique d'expansion des variables :
 * 1. Extrait le nom de la variable depuis la chaîne
 * 2. Gère les cas spéciaux (paramètres positionnels, $ isolé, etc.)
 * 3. Recherche la variable dans l'environnement
 * 4. Retourne une chaîne vide si la variable n'existe pas (comportement bash)
 *
 * @param str La chaîne contenant la variable à expanser
 * @param data Les données du shell (environnement, etc.)
 * @param start Pointeur vers l'index de début (sera mis à jour)
 * @param end Pointeur vers l'index de fin (sera mis à jour)
 * @return La valeur expansée allouée, ou NULL si erreur de parsing
 */
char *copy_var_content(char *str, t_shell *data, int *start, int *end)
{
	char *name_var;
	char *value;

	// Extraction du nom de la variable depuis la chaîne
	name_var = find_var(str, start, end);
	// Vérification et traitement des cas spéciaux
	// ! redondance sur le traitement des cas spéciaux
	value = handle_special_cases(name_var);
	if (value)
	{
		free(name_var);
		return (value);
	}
	else {
		// Sinon recherche de la variable dans l'environnement
		value = get_env_var_value(name_var, data->env);
		free(name_var);	
		// Si la variable n'existe pas, retourner une chaîne vide (comportement bash)
		if (!value)
			return (ft_strdup(""));
		return (value);
	}
}
