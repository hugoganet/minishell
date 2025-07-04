#include "expansion.h"
#include "minishell.h"

/**
 * @brief Traverse l'AST et expanse les variables dans les arguments des commandes.
 *
 * @param node Le nœud racine de l'AST à traiter.
 * @param shell La structure principale du shell contenant l'environnement et le statut.
 */
void expand_ast_arguments(t_ast *node, t_shell *shell)
{
	int i;
	char *expanded_arg;

	if (!node)
		return;
	// Parcourt l'AST en profondeur d'abord (post-ordre)
	expand_ast_arguments(node->left, shell);
	expand_ast_arguments(node->right, shell);
	// Si le nœud est une commande avec des arguments, on les expanse
	if (node->type == CMD && node->args)
	{
		i = 0;
		while (node->args[i])
		{
			// Appelle la fonction d'expansion pour chaque argument
			expanded_arg = expand_variables(node->args[i], shell->env_list, shell->last_exit_status);
			if (expanded_arg)
			{
				// Si l'argument expansé n'est pas vide, on le remplace dans l'AST
				free(node->args[i]);
				node->args[i] = expanded_arg;
			}
			i++;
		}
	}
}
