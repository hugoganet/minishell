
#include "minishell.h"

/**
 * @brief Ignore les espaces et tabulations à partir d'un index donné
 *
 * Avance l'index jusqu'au premier caractère non espace ou non tabulation.
 * Cette fonction protège contre les accès mémoire invalides en vérifiant
 * la validité des paramètres et l'existence du caractère courant.
 *
 * @param input La chaîne d'entrée à analyser (doit être non-NULL)
 * @param i Pointeur vers l'index à avancer (doit être non-NULL)
 */
void skip_spaces(char *input, int *i)
{
	if (!input || !i)
		return;
	while (input[*i] && (input[*i] == ' ' || input[*i] == '\t'))
		(*i)++;
}
/**
 * @brief Vérifie si le caractère courant est un séparateur de token
 *
 * Utilisé pour détecter les limites des mots, quotes ou opérateurs.
 * Un délimiteur marque la fin d'un token et le début d'un nouveau.
 *
 * Les délimiteurs reconnus sont :
 * - Espaces et tabulations (séparent les arguments)
 * - Opérateurs de redirection (<, >)
 * - Pipe (|)
 * - Fin de chaîne (\0)
 *
 * @param c Le caractère à tester
 * @return true si c'est un séparateur (espace, tab, symbole shell), false sinon
 */
bool is_token_delim(char c)
{
	return (c == ' ' || c == '\t' || c == '<' || c == '>' || c == '|' || c == '\0');
}

/**
 * @brief Lit un segment de texte jusqu'à la prochaine quote ou délimiteur
 *
 * Cette fonction auxiliaire extrait un mot simple (sans quotes) en s'arrêtant
 * dès qu'elle rencontre une quote ou un délimiteur de token.
 *
 * @param input La chaîne d'entrée à analyser
 * @param i Pointeur vers l'index courant (sera avancé)
 * @return char* Segment alloué ou NULL en cas d'erreur
 */
static char *read_word_segment(char *input, int *i)
{
	int start;

	start = *i;
	while (input[*i] && input[*i] != '\'' && input[*i] != '"' && !is_token_delim(input[*i]))
		(*i)++;
	return (ft_substr(input, start, *i - start));
}

/**
 * @brief Concatène deux chaînes en libérant les anciennes
 *
 * Fonction utilitaire pour la concaténation sécurisée avec gestion mémoire.
 * Libère automatiquement les deux chaînes d'origine après concaténation.
 *
 * @param result Chaîne de résultat (sera libérée)
 * @param segment Nouveau segment à ajouter (sera libéré)
 * @return char* Nouvelle chaîne concaténée ou NULL en cas d'erreur
 */
static char *join_and_free(char *result, char *segment)
{
	char *temp;

	if (!result)
		return (segment);
	temp = ft_strjoin(result, segment);
	free(result);
	free(segment);
	return (temp);
}
/**
 * @brief Lit un segment (quote ou mot) à partir de la position courante
 *
 * Détermine le type de segment à lire et appelle la fonction appropriée.
 * Gère à la fois les segments entre quotes et les mots simples.
 *
 * @param input La chaîne d'entrée à analyser
 * @param i Pointeur vers l'index courant (sera avancé)
 * @return `char *` Segment lu ou `NULL` en cas d'erreur
 */
static char *read_next_segment(char *input, int *i)
{
	if (input[*i] == '\'' || input[*i] == '"')
		return (parse_quoted_token(input, i));
	else
		return (read_word_segment(input, i));
}

/**
 * @brief Lit un token complet composé de plusieurs segments adjacents
 *
 * Cette fonction concatène tous les segments adjacents (quotes vides, quotes
 * pleines, et mots) en un seul token. Elle respecte le comportement bash où
 * des éléments comme ""''echo ou "hello"world sont traités comme un seul mot.
 *
 * Exemples de concaténation :
 * - ""''echo → "echo" (quotes vides + mot)
 * - "hello"world → "helloworld" (quote + mot)
 * - 'a'"b"c → "abc" (multiple quotes + mot)
 *
 * @param input La chaîne d'entrée utilisateur
 * @param i Pointeur vers l'index courant (sera mis à jour au fur et à mesure)
 * @return char* Token complet alloué, ou NULL en cas d'erreur mémoire
 */
static char *read_complete_token(char *input, int *i)
{
	char *result;
	char *segment;

	result = NULL;
	/* Parcourt et concatène tous les segments adjacents */
	while (input[*i] && !is_token_delim(input[*i]))
	{
		segment = read_next_segment(input, i);
		if (!segment)
		{
			free(result);
			return (NULL);
		}
		result = join_and_free(result, segment);
		if (!result)
			return (NULL);
	}
	return (result);
}
/**
 * @brief Lit un opérateur spécial (>> / <<)
 *
 * @param input Ligne d’entrée
 * @param i Index à avancer
 * @return char* Opérateur alloué
 */
static char *read_operator(char *input, int *i)
{
	char *op;
	if ((input[*i] == '<' && input[*i + 1] == '<') || (input[*i] == '>' && input[*i + 1] == '>'))
	{
		/* Opérateur de 2 caractères (<<, >>) */
		op = ft_substr(input, *i, 2);
		if (!op)
			return (NULL);
		*i += 2;
	}
	else
	{
		/* Opérateur simple (<, >, |) */
		op = ft_substr(input, *i, 1);
		if (!op)
			return (NULL);
		(*i)++;
	}
	return (op);
}
/**
 * @brief Concatène une redirection et le fichier cible en un seul token
 *
 * Exemple : si input = "> out.txt", retourne ">out.txt"
 *
 * @param input Ligne d’entrée utilisateur
 * @param i Pointeur vers l’index courant (sera avancé)
 * @return char* Contenu combiné (redir + file), ou NULL si erreur
 */
static char *read_redir_and_file(char *input, int *i)
{
	char *op;
	char *file;
	char *combined;

	op = read_operator(input, i);
	if (!op)
		return (NULL);
	skip_spaces(input, i);
	file = read_complete_token(input, i);
	if (!file)
	{
		free(op);
		return (NULL);
	}
	combined = ft_strjoin(op, file);
	free(op);
	free(file);
	return (combined);
}
/**
 * @brief Lit un token complet (quote, mot ou opérateur) à partir de l’index donné
 *
 * Crée systématiquement un nouveau token dès qu’un bloc (mot, quote ou opérateur) est détecté.
 *
 * @param input Ligne utilisateur
 * @param i Pointeur vers l’index actuel (sera mis à jour)
 * @return t_token* Un token alloué, ou NULL en cas d’erreur
 */
static t_token *get_next_token(char *input, int *i)
{
	char *content;
	t_token_type type;

	if ((input[*i] == '<' || input[*i] == '>') && input[*i] != '\0')
		content = read_redir_and_file(input, i);
	else if (!is_token_delim(input[*i]) && input[*i])
		content = read_complete_token(input, i);
	else
		content = read_operator(input, i);
	/* Vérification d'erreur de lecture */
	if (!content)
		return (NULL);
	/* Détermination du type et création du token */
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

	new = get_next_token(input, i);
	if (!new && input[*i])
	{
		ft_putendl_fd("minishell: error: failed to tokenize input", 2);
		free_token_list(*head);
		return (-1);
	}
	if (new)
		append_token(head, last, new);
	return (0);
}

/**
 * @brief Effectue le post-traitement des tokens
 *
 * Applique les transformations finales sur la liste de tokens :
 * - Affinement des types (CMD, ARG, etc.)
 * - Validation syntaxique
 *
 * @param head Pointeur vers la tête de liste
 * @return int 0 en cas de succès, -1 en cas d'erreur syntaxique
 */
static int finalize_tokens(t_token **head)
{
	refine_token_types(*head);
	if (validate_token_sequence(*head))
	{
		free_token_list(*head);
		return (-1);
	}
	return (0);
}

/**
 * @brief Fonction principale de découpage : transforme la ligne en une liste de tokens
 *
 * @param input Chaîne entrée par l’utilisateur
 * @return t_token* Liste chaînée des tokens
 */
t_token *tokenize(char *input)
{
	t_token *head;
	t_token *last;
	int i;

	head = NULL;
	last = NULL;
	i = 0;
	skip_spaces(input, &i);
	while (input[i])
	{
		if (process_and_add_token(input, &i, &head, &last) == -1)
			return (NULL);
		skip_spaces(input, &i);
	}
	if (finalize_tokens(&head) == -1)
		return (NULL);
	return (head);
}
