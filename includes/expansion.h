#ifndef EXPANSION_H
#define EXPANSION_H

#include "minishell.h" // Ou les en-têtes nécessaires
#include <stdbool.h>

/**
 * @brief Structure pour gérer l'état de l'expansion.
 * 	  Contient la chaîne d'entrée, la chaîne de sortie,
 *    l'index de lecture, et les états des quotes.
 *    Cette structure est utilisée pour suivre l'état de l'expansion
 *    des variables dans la chaîne d'entrée.
 *
 * @param input La chaîne d'entrée à traiter.
 * @param output La chaîne de sortie après expansion.
 * @param i L'index de lecture dans la chaîne d'entrée.
 * @param in_single_quotes Flag pour l'état des quotes simples.
 * @param in_double_quotes Flag pour l'état des quotes doubles.
 */
typedef struct s_expansion_state
{
	const char *input;
	char *output;
	int i;
	bool in_single_quotes;
	bool in_double_quotes;
} t_expansion_state;

// --- Fonctions principales (expansion.c) ---

/**
 * @brief Point d'entrée principal pour l'expansion des variables.
 *        Orchestre l'analyse de la chaîne et la substitution des variables.
 *
 * @param input La chaîne de caractères à traiter.
 * @param env_list La liste des variables d'environnement.
 * @param last_exit_status Le code de retour de la dernière commande.
 * @return Une nouvelle chaîne allouée avec les variables expansées.
 */
char *expand_variables(const char *input, t_env *env_list, int last_exit_status);

// --- Gestion des quotes (expansion_quotes.c) ---

/**
 * @brief Met à jour l'état des quotes (simples/doubles) en fonction du
 *        caractère courant.
 *
 * @param state L'état actuel de l'expansion.
 */
void expansion_update_quote_state(t_expansion_state *state);

// --- Gestion des variables spéciales (expansion_specials.c) ---

/**
 * @brief Gère l'expansion d'une variable spéciale (ex: $?, $$).
 *
 * @param state L'état actuel de l'expansion.
 * @param last_exit_status Le code de retour de la dernière commande.
 * @return Le contenu de la variable spéciale, ou NULL si non applicable.
 */
char *expand_special_variable(t_expansion_state *state, int last_exit_status);

// --- Fonctions utilitaires (expansion_utils.c) ---

/**
 * @brief Extrait le nom d'une variable à partir de la position actuelle.
 *        Ex: pour "$USER_VAR", extrait "USER_VAR".
 *
 * @param input La chaîne d'entrée.
 * @param start_index L'index de début (après le '$').
 * @return Le nom de la variable (chaîne allouée).
 */
char *extract_variable_name(const char *input, int start_index);

/**
 * @brief Recherche la valeur d'une variable dans l'environnement.
 *
 * @param var_name Le nom de la variable à rechercher.
 * @param env_list L'environnement sous forme de liste chaînée.
 * @return La valeur de la variable (chaîne allouée), ou NULL si non trouvée.
 */
char *get_env_variable(const char *var_name, t_env *env_list);

/**
 * @brief Traverse l'AST et expanse les variables dans les arguments des commandes.
 *
 * @param node Le nœud racine de l'AST à traiter.
 * @param shell La structure principale du shell contenant l'environnement et le statut.
 */
void expand_ast_arguments(t_ast *node, t_shell *shell);

bool is_translated_string(const char *input, int dollar_pos);
char *extract_translated_string(const char *input, int dollar_pos, int *end_pos);
void append_to_output(t_expansion_state *state, const char *to_add);
void append_and_free(t_expansion_state *state, char *value);

#endif
