/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expand.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 19:54:08 by elaudrez          #+#    #+#             */
/*   Updated: 2025/06/08 14:25:29 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Cas ${VAR}
void	get_name_brace(t_ast *node, int *i, int *end, int *name_start)
{
	(*i)++;
		*name_start = *i; // Indiquer start pour mesurer la len
	if (ft_isdigit(node->str[*i])) //Si le premier est un chiffre -> invalide
		return ;
	while (node->str[*i] && node->str[*i] != '}') //tant qu'on a pas la quote fermante
	{
		if (!ft_isalnum(node->str[*i]) && node->str[*i] != '_') //Si le reste n'est pas un chiffre ou une lettre ou underscore -> invalide
			return ;
		(*i)++;
	}
	if (node->str[*i] != '}') // Si pas de quote fermante, invalide
		return ;
	*end = *i + 1; //Placer end juste apres le }
}

void	get_name(t_ast *node, int *i, int *end, int *name_start)
{
	*name_start = *i;
		if (ft_isdigit(node->str[*i])) //Si commence par un chiffre -> invalide
			return ;
		while (node->str[*i] && (ft_isalnum(node->str[*i]) || node->str[*i] == '_'))
			(*i)++;
		*end = *i;
}

/* Extraire le nom de la variable appelée dans l'input */
char	*find_var(t_ast *node, int *start, int *end)
{
	int	i;
	int	len;
	int	name_start;
	char *name_var;

	i = 0;
	
	while(node->str[i] && node->str[i] != '$')
		i++;
	if (!node->str[i])
		return (NULL);
	*start = i;
	i++;
	// Cas ${VAR}
	if (node->str[i] == '{')
		get_name_brace(node, &i, end, &name_start);
	// Cas $VAR
	else
		get_name(node, &i, end, &name_start);
	len = i - name_start; //Definir la longueur de la substring qu'il faudra chercher dans env ex : 10 - 5 USER et pas ${USER}
	name_var = malloc((len + 1) * sizeof(char));
	if (!name_var)
		return (NULL);
	ft_strlcpy(name_var, &node->str[name_start], len + 1); //ajouter le char \0 a la fin . Est ce que ca serait pas mieux d'utiliser substr ?
	return (name_var);
}

char	*copy_var_content(t_ast *node, t_shell *data, int *start, int *end)
{
	int		i;
	char	*name_var;
	int		len;
	char	*var;
	
	i = 0;
	name_var = find_var(node, start, end); // Recuperer variable appelée dans le terminal
	if(!name_var)
		return (NULL);
	len = ft_strlen(name_var);
	while (data->env[i]) //Parcourir tableau des variables d'env pour trouver concordance avec name_var
	{
		if (ft_strncmp(data->env[i], name_var, len) == 0 && data->env[i][len] == '=') // Si meme nom trouvé, strncmp renvoie 0 donc on arrete de parcourir le tableau et on sort de la boucle
			break;
		i++;
	}
	if (data->env[i] == NULL) // Si on a parcouru tout le tableau -> pas de concordance trouvée, nom invalide
		return (NULL);

	var = ft_strdup(&data->env[i][len + 1]); // Recuperer le contenu sans le nom ex : USER=elaudrez, prendre que elaudrez donc len = taille du nom de la variable +1 pour le '='
	return (var);
}

void	join_str(t_ast *node, t_shell *data)
{
	char	*var;
	char	*prefix; // Chaine avant la variable a traiter
	char	*suffix; // Chaine apres 
	char	*final_str; // Chaine qui sera composée des 3 chaines concaténées
	char	*tmp;
	int		start; // Savoir a partir d'ou le traitement de la variable commence donc ($).
	int		end; // Et ou elle s'arrete donc soit a la fin, soit }

	start = 0;
	end = 0;
	while (ft_strchr(node->str, '$'))
	{
		var = copy_var_content(node, data, &start, &end);
		if (!var)
			return ;
		prefix = ft_substr(node->str, 0, start);
		suffix = ft_substr(node->str, end, ft_strlen(node->str) - end); //copier le nb de charactere qui reste, donc len string - l'index end (qui se trouve a la fin) ex : 15 - 10.  	
		tmp = ft_strjoin(prefix, var);
		final_str = ft_strjoin(tmp, suffix);
		free(tmp);
		free(prefix);
		free(suffix);
		node->str = final_str;
	}
}

void	expand_vars(t_ast *node, t_shell *data)
{
	if(!node)
		return ;
	if (node->type == ARG)
	{
		if (ft_strchr(node->str, '$'))
		{
			if (which_quote(node) == 1)
				join_str(node, data);	
		}
	}
	expand_vars(node->left, data);
	expand_vars(node->right, data);
}

