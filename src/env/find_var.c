
#include "minishell.h"

/**
 * @brief Extrait le nom de variable depuis une position donnée.
 *
 * Cette fonction gère les différents cas de noms de variables :
 * - Paramètres positionnels ($1, $2, etc.) → retourne chaîne vide
 * - Nom vide (juste $) → retourne chaîne vide
 * - Nom normal → extrait et retourne le nom
 *
 * @param str La chaîne d'origine
 * @param name_start L'index de début du nom
 * @param len La longueur du nom à copier
 * @return Une copie allouée du nom ou chaîne vide selon les cas
 */
char *extract_var_name(char *str, int name_start, int len)
{
	// Si le nom est un paramètre positionnel, on retourne une chaîne vide
	if (len == 1 && ft_isdigit(str[name_start]))
		return (ft_strdup(""));
	// Si le nom est vide (par exemple juste un `$`), on retourne une chaîne vide
	if (len == 0)
		return (ft_strdup(""));
	// Sinon, on extrait le nom de variable
	return (ft_substr(str, name_start, len));
}

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
	*name_start = *i;
	if (ft_isdigit(str[*i]))
	{
		(*i)++;
		if (str[*i] == '}')
			*end = *i + 1;
		else
			*end = *i;
		return;
	}
	while (str[*i] && str[*i] != '}')
	{
		if (!ft_isalnum(str[*i]) && str[*i] != '_')
			return;
		(*i)++;
	}
	if (str[*i] != '}')
		return;
	*end = *i + 1;
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
	// Si on commence par un chiffre, c'est un paramètre positionnel
	if (ft_isdigit(str[*i]))
	{
		// Avancer d'un seul chiffre pour le paramètre positionnel ($0, $1, etc.)
		(*i)++;
		*end = *i;
		return;
	}
	// Parser le nom de variable classique
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	*end = *i;
}

/**
 * @brief Extrait le nom d’une variable dans une chaîne `$VAR` ou `${VAR}`.
 *
 * @param str Chaîne source.
 * @param start Index de début du token `$`.
 * @param end Index de fin après la variable.
 * @return Copie allouée du nom de variable, ou "$" si invalide, ou "" si paramètre positionnel.
 */
char *find_var(char *str, int *start, int *end)
{
	int i;
	int len;
	int name_start;
	i = 0;
	// Chercher le premier `$` dans la chaîne
	while (str[i] && str[i] != '$')
		i++;
	*start = i;
	i++;
	// Si on n'a pas trouvé de `$`, retourner "$"
	if (!str[i])
	{
		*end = i;
		return (ft_strdup("$"));
	}
	// Si le caractère après `$` n'est pas valide, retourner "$"
	if (!is_valid_var_start(str[i]))
	{
		*end = i;
		return (ft_strdup("$"));
	}
	// Si on a un `{`, on traite le nom entre accolades `${VAR}`
	if (str[i] == '{')
		get_name_brace(str, &i, end, &name_start);
	// Sinon, on traite le nom classique `$VAR`
	else
		get_name(str, &i, end, &name_start);
	// Calculer la longueur du nom extrait
	len = i - name_start;
	return (extract_var_name(str, name_start, len));
}