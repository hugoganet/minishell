/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expand.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 19:54:08 by elaudrez          #+#    #+#             */
/*   Updated: 2025/06/26 12:41:57 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Cas ${VAR}
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

void get_name(char *str, int *i, int *end, int *name_start)
{
	*name_start = *i;
	if (ft_isdigit(str[*i])) // Si commence par un chiffre -> invalide
		return;
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	*end = *i;
}

/* Extraire le nom de la variable appelée dans l'input */
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

char *join_str(char *str, t_shell *data)
{
	char *var;
	char *prefix;	 // Chaine avant la variable a traiter
	char *suffix;	 // Chaine apres
	char *final_str; // Chaine qui sera composée des 3 chaines concaténées
	char *tmp;
	int start; // Savoir a partir d'ou le traitement de la variable commence donc ($).
	int end;   // Et ou elle s'arrete donc soit a la fin, soit }

	start = 0;
	end = 0;
	while (ft_strchr(str, '$'))
	{
		var = copy_var_content(str, data, &start, &end);
		if (!var)
			break; // Si pas de variable trouvée, on sort de la boucle
		prefix = ft_substr(str, 0, start);
		suffix = ft_substr(str, end, ft_strlen(str) - end); // copier le nb de charactere qui reste, donc len string - l'index end (qui se trouve a la fin) ex : 15 - 10.
		tmp = ft_strjoin(prefix, var);
		final_str = ft_strjoin(tmp, suffix);
		free(tmp);
		free(var);
		free(prefix);
		free(suffix);
		free(str);		 // Libérer l'ancienne chaîne
		str = final_str; // Mettre à jour la chaîne
	}
	return (str);
}

/**
 * @brief Retire les guillemets d'une chaîne
 * @param str Chaîne à traiter
 * @return Nouvelle chaîne sans guillemets (allouée dynamiquement)
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

	result = malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (!result)
		return (NULL);

	i = 0;
	j = 0;
	in_single_quote = false;
	in_double_quote = false;

	while (str[i])
	{
		if (str[i] == '\'' && !in_double_quote)
		{
			in_single_quote = !in_single_quote;
			i++;
		}
		else if (str[i] == '"' && !in_single_quote)
		{
			in_double_quote = !in_double_quote;
			i++;
		}
		else
			result[j++] = str[i++];
	}
	result[j] = '\0';

	return (result);
}

void expand_vars(t_ast *node, t_shell *data)
{
	int i;
	int j;
	char *expanded;
	char *unquoted;

	i = 1;
	if (!node)
		return;
	if (node->type == CMD)
	{
		while (node->args[i])
		{
			// Étape 1: Expansion des variables
			j = 0;
			while (node->args[i][j])
			{
				if (node->args[i][j] == '$') // Vérifier si on a une variable à expanser
				{
					if (to_exp(node->args[i])) // Vérifier si on doit expanser
					{
						expanded = join_str(ft_strdup(node->args[i]), data);
						if (expanded)
						{
							free(node->args[i]);
							node->args[i] = expanded;
						}
						break; // Après expansion, on sort de la boucle car la chaîne a été modifiée
					}
				}
				j++;
			}

			// Étape 2: Retirer les guillemets
			unquoted = remove_quotes(node->args[i]);
			if (unquoted)
			{
				free(node->args[i]);
				node->args[i] = unquoted;
			}

			i++;
		}
	}
	expand_vars(node->left, data);
	expand_vars(node->right, data);
}
