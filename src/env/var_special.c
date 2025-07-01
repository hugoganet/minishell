/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_special.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 12:00:00 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/01 11:44:48 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Vérifie si le nom correspond à un paramètre positionnel.
 *
 * Les paramètres positionnels sont des variables de la forme $1, $2, etc.
 * Dans minishell, ils sont généralement vides car nous n'avons pas d'arguments
 * de script, mais nous devons les gérer pour la compatibilité avec bash.
 *
 * @param name Le nom de la variable à vérifier (sans le $)
 * @return true si c'est un paramètre positionnel, false sinon
 */
bool is_positional_param(const char *name)
{
	// Vérification : nom d'un seul caractère ET c'est un chiffre
	return (ft_strlen(name) == 1 && ft_isdigit(name[0]));
}

/**
 * @brief Vérifie si la variable est un cas spécial nécessitant un traitement particulier.
 *
 * Les cas spéciaux incluent :
 * - Paramètres positionnels ($1, $2, etc.)
 * - Chaîne vide (juste $)
 * - Dollar isolé non suivi d'un nom valide
 *
 * @param name Le nom de la variable à vérifier
 * @return true si c'est une variable spéciale, false sinon
 */
bool is_special_var(const char *name)
{
	if (!name)
		return (true);
	// Paramètre positionnel
	if (is_positional_param(name))
		return (true);
	// Chaîne vide (juste un $)
	if (ft_strlen(name) == 0)
		return (true);
	// Dollar isolé non suivi de nom valide
	if (ft_strlen(name) == 1 && name[0] == '$')
		return (true);
	// Variable normale
	return (false);
}

/**
 * @brief Gère les cas spéciaux de variables et retourne la valeur appropriée.
 *
 * Cette fonction centralise le traitement de tous les cas spéciaux :
 * - Paramètres positionnels → chaîne vide (car pas d'args de script)
 * - Chaîne vide → chaîne vide
 * - Dollar isolé → garde le $ littéralement
 *
 * @param name Le nom de la variable spéciale
 * @return La chaîne allouée correspondant au cas spécial, ou NULL si pas spécial
 */
char *handle_special_cases(const char *name)
{
	if (!name)
		return (NULL);
	// Paramètres positionnels → chaîne vide
	if (is_positional_param(name))
		return (ft_strdup(""));
	// Nom vide → chaîne vide
	if (ft_strlen(name) == 0)
		return (ft_strdup(""));
	// Dollar isolé → garde le $ tel quel
	if (ft_strlen(name) == 1 && name[0] == '$')
		return (ft_strdup("$"));
	// Pas un cas spécial
	return (NULL);
}

/**
 * @brief Vérifie si le caractère peut commencer un nom de variable valide.
 *
 * Un nom de variable peut commencer par :
 * - Une lettre (a-z, A-Z)
 * - Un underscore (_)
 * - Une accolade ouvrante ({) pour ${VAR}
 * - Un point d'interrogation (?) pour $?
 * - Un chiffre (0-9) pour les paramètres positionnels
 *
 * @param c Le caractère à vérifier
 * @return true si le caractère peut commencer un nom de variable, false sinon
 */
bool is_valid_var_start(char c)
{
	return (ft_isalpha(c) || c == '_' || c == '{' || c == '?' || ft_isdigit(c));
}
