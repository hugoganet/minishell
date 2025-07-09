/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_core.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 00:00:00 by hugoganet         #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/07/08 18:06:00 by hugoganet        ###   ########.fr       */
=======
/*   Updated: 2025/07/09 11:24:52 by hugoganet        ###   ########.fr       */
>>>>>>> 932411b06464e72ed1555065287490f67d354db1
/*                                                                            */
/* ************************************************************************** */

#include "syntax.h"

/**
 * @brief Lit un token complet (quote, mot ou opérateur) à partir de l'index
 * donné
 *
 * Crée systématiquement un nouveau token dès qu'un bloc (mot, quote ou
 * opérateur) est détecté.
 * Gère les différents types de tokens : redirections combinées, mots
 * complets, opérateurs.
 *
 * @param input Ligne utilisateur
 * @param i Pointeur vers l'index actuel (sera mis à jour)
 * @return t_token* Un token alloué, ou NULL en cas d'erreur
 */
static t_token *get_next_token(char *input, int *i)
{
	char *content;
	t_token_type type;

	// Si l'input est une redir, on extrait la redir 
	// (simple ou double et on concatène avec le fichier cible, cleané.
	// `<<.  "''"'""file.txt` devient `<<file.txt`
	if ((input[*i] == '<' || input[*i] == '>') && input[*i] != '\0')
		content = read_redir_and_file(input, i);
	// Si le charactère courant n'est un délimiteur (espaces, tabulations, |)
	// on clean la chaîne jusqu'au prochain délimiteur (WORD)
	else if (!is_token_delim(input[*i]) && input[*i])
		content = clean_token(input, i);
	// (à priori, cas impossible)
	else
		content = read_operator(input, i);
	if (!content)
		return (NULL);
	// On détermine le type du token en fonction de son contenu
	// à ce stade, le token est un WORD, une redir ou un pipe
	type = get_token_type(content);
	return (token_new(content, type));
}

/**
 * @brief Traite un token et l'ajoute à la liste si valide
 *
 * Cette fonction gère l'ajout d'un nouveau token à la liste chaînée
 * avec vérification d'erreur et nettoyage en cas d'échec.
 *
 * @param input La chaîne d'entrée (pour les messages d'erreur)
 * @param i Pointeur vers l'index courant
 * @param head Pointeur vers la tête de liste
 * @param last Pointeur vers le dernier élément
 * @return int 0 en cas de succès, -1 en cas d'erreur
 */
static int process_and_add_token(char *input, int *i, t_token **head,
								 t_token **last)
{
	t_token *new;

	// On traite le token à partir de l'index courant, i est mis à jour par référence
<<<<<<< HEAD
=======
	// et on récupère le token alloué.
	// à ce stade, tous les tokens sont des WORD, des redirections ou des pipes.
>>>>>>> 932411b06464e72ed1555065287490f67d354db1
	new = get_next_token(input, i);
	// Si le token est NULL, mais qu'il y a encore des caractères dans l'input,
	// cela signifie qu'il y a eu une erreur d'allocation ou de lecture
	if (!new && input[*i])
	{
		ft_putendl_fd("minishell: error: failed to tokenize input", 2);
		free_token_list(*head);
		return (-1);
	}
	// Si le token n'est pas NULL, on l'ajoute à la liste
	if (new)
		append_token(head, last, new);
	return (0);
}

/**
 * @brief Effectue le post-traitement des tokens
 *
 * Applique les transformations finales sur la liste de tokens :
<<<<<<< HEAD
 * 
 * - Affinement des types (CMD, ARG, etc.)
 * 
=======
 *
 * - Affinement des types (CMD, ARG, etc.)
 *
>>>>>>> 932411b06464e72ed1555065287490f67d354db1
 * - Validation syntaxique
 *
 * @param head Pointeur vers la tête de liste
 * @return int 0 en cas de succès, -1 en cas d'erreur syntaxique
 */
static int finalize_tokens(t_token **head)
{
	// On requalifie les tokens WORD en CMD ou ARG
	refine_token_types(*head);
	// On check la séquence de tokens pour détecter les erreurs de syntaxe
	// (ex: opérateur logique au début ou à la fin, opérateurs consécutifs)
	if (validate_token_sequence(*head))
	{
		// Si une erreur de syntaxe est détectée, on libère la liste
		// et on retourne -1 pour indiquer l'erreur.
		free_token_list(*head);
		return (-1);
	}
	return (0);
}

/**
 * @brief Fonction principale de découpage : transforme la ligne en une
 * liste de tokens
 *
 * Parcourt la chaîne d'entrée caractère par caractère, identifie les tokens
 * et les ajoute à une liste chaînée. Effectue ensuite le post-traitement
 * pour affiner les types et valider la syntaxe.
 *
 * @param input Chaîne entrée par l'utilisateur
 * @return t_token* Liste chaînée des tokens, ou NULL en cas d'erreur
 */
t_token *tokenize(char *input)
{
	t_token *head;
	t_token *last;
	int i;

	head = NULL;
	last = NULL;
	i = 0;
	// On ignore les espaces initiaux en mettant à jour i.
	skip_spaces(input, &i);
	// On parcourt la chaîne d'entrée jusqu'à la fin pour traiter chaque token un par un
	while (input[i])
	{
		// On passe i par référence pour qu'il soit mis à jour dans process_and_add_token
		if (process_and_add_token(input, &i, &head, &last) == -1)
			return (NULL);
		// On ignore les espaces entre chaque token en mettant à jour i.
		skip_spaces(input, &i);
	}
	// Si on a atteint la fin de la chaîne sans erreur, on fait un dernier traitement
	// pour affiner les types et valider la syntaxe.
	if (finalize_tokens(&head) == -1)
		return (NULL);
	return (head);
}
