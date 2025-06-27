/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expand.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 19:54:08 by elaudrez          #+#    #+#             */
/*   Updated: 2025/06/27 15:56:38 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Détecte et valide un nom de variable entre accolades : ${VAR}
 *
 * Cette fonction extrait les bornes de la variable `${VAR}` dans la chaîne donnée,
 * en vérifiant que le nom est valide (lettres, chiffres, underscores uniquement).
 *
 * @param str         La chaîne d’entrée.
 * @param i           Index courant (début du parsing, sera modifié).
 * @param end         Index de fin à renseigner (après la }).
 * @param name_start  Index de début du nom de variable (après le {).
 */
void get_name_brace(char *str, int *i, int *end, int *name_start)
{
	(*i)++;
	*name_start = *i;		 // Indiquer start pour mesurer la len
	if (ft_isdigit(str[*i])) // Si le premier est un chiffre -> invalide
		return;
	while (str[*i] && str[*i] != '}') // tant qu'on a pas la quote fermante
	{
		if (!ft_isalnum(str[*i]) && str[*i] != '_') // Si le reste n'est pas un chiffre ou une lettre ou underscore -> invalide
			return;
		(*i)++;
	}
	if (str[*i] != '}') // Si pas de quote fermante, invalide
		return;
	*end = *i + 1; // Placer end juste apres le }
}
/**
 * @brief Détecte un nom de variable classique (sans accolades) : $VAR
 *
 * Cette fonction extrait les bornes du nom de la variable à partir de l’index donné,
 * en s'arrêtant sur le premier caractère invalide (non alphanumérique/underscore).
 *
 * @param str         La chaîne d’entrée.
 * @param i           Index courant (sera modifié jusqu’à la fin du nom).
 * @param end         Index de fin du nom.
 * @param name_start  Index de début du nom.
 */

void get_name(char *str, int *i, int *end, int *name_start)
{
	*name_start = *i;
	if (ft_isdigit(str[*i])) // Si commence par un chiffre -> invalide
		return;
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	*end = *i;
}

/**
 * @brief Extrait le nom d’une variable présente dans une chaîne `$VAR` ou `${VAR}`
 *
 * Recherche le prochain `$`, et récupère le nom de la variable (sans le `$` ni `{}`),
 * en renvoyant une copie allouée du nom.
 *
 * @param str   Chaîne à analyser.
 * @param start Index de début du token `$`.
 * @param end   Index de fin après la variable.
 * @return Nom de la variable (ex: "USER"), à libérer après usage. NULL si erreur.
 */
char *find_var(char *str, int *start, int *end)
{
	int i;
	int len;
	int name_start;
	char *name_var;

	i = 0;

	while (str[i] && str[i] != '$')
		i++;
	if (!str[i])
		return (NULL);
	*start = i;
	i++;
	// Cas ${VAR}
	if (str[i] == '{')
		get_name_brace(str, &i, end, &name_start);
	// Cas $VAR
	else
		get_name(str, &i, end, &name_start);
	len = i - name_start; // Definir la longueur de la substring qu'il faudra chercher dans env ex : 10 - 5 USER et pas ${USER}
	name_var = malloc((len + 1) * sizeof(char));
	if (!name_var)
		return (NULL);
	ft_strlcpy(name_var, &str[name_start], len + 1); // ajouter le char \0 a la fin . Est ce que ca serait pas mieux d'utiliser substr ?
	return (name_var);
}

/**
 * @brief Récupère la valeur d’une variable d’environnement à partir de son appel dans une chaîne.
 *
 * Recherche un appel de variable via `$VAR` ou `${VAR}` dans `str`, puis renvoie la valeur
 * correspondante dans `data->env`, ou NULL si non trouvée.
 *
 * @param str   Chaîne contenant l’appel de variable.
 * @param data  Données du shell contenant `env`.
 * @param start Adresse où stocker l’index de début du `$`.
 * @param end   Adresse où stocker l’index de fin après la variable.
 * @return Valeur de la variable (allouée dynamiquement), ou NULL si non trouvée.
 */
char *copy_var_content(char *str, t_shell *data, int *start, int *end)
{
	int i;
	char *name_var;
	int len;
	char *var;

	i = 0;
	name_var = find_var(str, start, end); // Recuperer variable appelée dans le terminal
	if (!name_var)
		return (NULL);
	len = ft_strlen(name_var);
	while (data->env[i]) // Parcourir tableau des variables d'env pour trouver concordance avec name_var
	{
		if (ft_strncmp(data->env[i], name_var, len) == 0 && data->env[i][len] == '=') // Si meme nom trouvé, strncmp renvoie 0 donc on arrete de parcourir le tableau et on sort de la boucle
			break;
		i++;
	}
	free(name_var);
	if (data->env[i] == NULL) // Si on a parcouru tout le tableau -> pas de concordance trouvée, nom invalide
		return (NULL);

	var = ft_strdup(&data->env[i][len + 1]); // Recuperer le contenu sans le nom ex : USER=elaudrez, prendre que elaudrez donc len = taille du nom de la variable +1 pour le '='
	return (var);
}

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
 * @brief Supprime les quotes simples et doubles d’une chaîne tout en respectant les règles du shell.
 *
 * Cette fonction conserve le contenu entre les quotes, mais supprime les caractères
 * de quote eux-mêmes (pas d’expansion entre quotes simples, expansion autorisée dans les doubles).
 *
 * @param str Chaîne originale (non modifiée).
 * @return Une nouvelle chaîne sans les quotes (à libérer après usage).
 */
char *remove_quotes(char *str)
{
	int i;
	int j;
	char *result;
	bool in_single_quote;
	bool in_double_quote;

	if (!str)
		return (NULL);
	result = ft_calloc(sizeof(char), (ft_strlen(str) + 1));
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	in_single_quote = false;
	in_double_quote = false;
	while (str[i])
	{
		// Toggle quote simple si hors quote double
		if (str[i] == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		// Toggle quote double si hors quote simple
		else if (str[i] == '"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		else
			result[j++] = str[i];
		i++;
	}
	return (result);
}

/**
 * @brief Gère l'expansion de la variable $? qui renvoie le statut de sortie de la dernière commande
 *
 * @param str   Chaîne contenant $?
 * @param data  Données du shell contenant last_exit_status
 * @param start Adresse où stocker l'index de début du $?
 * @param end   Adresse où stocker l'index de fin après $?
 * @return Valeur du dernier code de sortie sous forme de chaîne allouée dynamiquement
 */
char *expand_exit_status(char *str, t_shell *data, int *start, int *end)
{
	int i;
	char *exit_status;

	i = 0;
	// Trouver la première occurrence de $?
	while (str[i] && (str[i] != '$' || str[i + 1] != '?'))
		i++;
	if (!str[i])
		return (NULL);
	*start = i;
	*end = i + 2; // $?

	// Convertir l'entier en chaîne
	exit_status = ft_itoa(data->last_exit_status);
	return (exit_status);
}

/**
 * @brief Effectue l'expansion d'une variable d'environnement dans une chaîne.
 *
 * Cette fonction cherche la première occurrence de `$VAR` dans la chaîne `str`,
 * en extrait la valeur correspondante depuis la liste d'environnement `data`,
 * puis reconstruit la chaîne en concaténant :
 *   - le préfixe (avant `$`)
 *   - la valeur de la variable
 *   - le suffixe (après la variable)
 * Cela est répété tant que d'autres `$` sont trouvés.
 *
 * @param str Chaîne à traiter (libérée automatiquement).
 * @param data Structure contenant les variables d’environnement.
 * @param str Pointeur vers un pointeur vers la chaîne à modifier.
 * @return Nouvelle chaîne avec toutes les variables expansées.
 */
char *join_str(char *str, t_shell *data)
{
	char *prefix;
	char *var;
	char *suffix;
	char *tmp;
	char *final;
	int start;
	int end;

	start = 0;
	end = 0;
	while (ft_strchr(str, '$'))
	{
		// Vérifier d'abord si $? est présent
		var = expand_exit_status(str, data, &start, &end);
		if (!var)
			var = copy_var_content(str, data, &start, &end);
		if (!var)
			break;
		prefix = ft_substr(str, 0, start);
		suffix = ft_substr(str, end, ft_strlen(str) - end);
		tmp = ft_strjoin(prefix, var);
		final = ft_strjoin(tmp, suffix);
		free_temp(prefix, var, suffix, tmp);
		free(str);
		str = final;
	}
	return (str);
}

static void expand_one_arg(char **arg, t_shell *data)
{
	char *expanded;
	char *unquoted;
	int j;

	j = 0;
	while ((*arg)[j])
	{
		if ((*arg)[j] == '$' && to_exp(*arg))
		{
			expanded = join_str(ft_strdup(*arg), data);
			if (expanded)
			{
				free(*arg);
				*arg = expanded;
			}
			break;
		}
		j++;
	}
	unquoted = remove_quotes(*arg);
	if (unquoted)
	{
		free(*arg);
		*arg = unquoted;
	}
}

/**
 * @brief Applique l’expansion sur tous les arguments d’un nœud AST (CMD uniquement).
 */
void expand_vars(t_ast *node, t_shell *data)
{
	int i;

	if (!node)
		return;
	if (node->type == CMD)
	{
		i = 1;
		while (node->args[i])
			expand_one_arg(&node->args[i++], data);
	}
	expand_vars(node->left, data);
	expand_vars(node->right, data);
}
