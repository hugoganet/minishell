#include "expansion.h"
#include "libft.h"

/**
 * @brief Gère l'expansion d'une variable spéciale (ex: $?, $0).
 *        Ne traite QUE les cas vraiment spéciaux.
 *
 * @param state L'état actuel de l'expansion.
 * @param last_exit_status Le code de retour de la dernière commande.
 * @return Le contenu de la variable spéciale, ou NULL si non applicable.
 */
char *expand_special_variable(t_expansion_state *state, int last_exit_status)
{
	char next_char;

	// Vérifie si on est à la fin de la chaîne d'entrée
	if (state->input[state->i + 1] == '\0')
		return (NULL);
	next_char = state->input[state->i + 1];
	// Variable spéciale $? (exit status)
	if (next_char == '?')
	{
		state->i += 2; // Avance l'index de 2 ($?)
		return (ft_itoa(last_exit_status));
	}
	// Variable spéciale $0 (nom du shell)
	if (next_char == '0')
	{
		state->i += 2; // Avance l'index de 2 ($0)
		return (ft_strdup("minishell"));
	}
	// Paramètres positionnels $1-$9 : pas supportés, retourner NULL
	// pour laisser handle_dollar gérer le cas
	return (NULL);
}
