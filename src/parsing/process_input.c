/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 17:52:43 by hugoganet         #+#    #+#             */
/*   Updated: 2025/05/30 17:09:30 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Traite la ligne de commande : tokenisation, parsing logique à venir.
 *
 * @param input La ligne brute saisie par l'utilisateur
 * @param shell Structure principale du shell
 */
void process_input(char *input, t_shell *shell)
{
	t_token *tokens;

	(void)shell; // Utile plus tard
	// Tokenisation de la ligne d'entrée
	tokens = tokenize(input);
	if (!tokens)
	{
		// Si la tokenisation échoue, on affiche un message d'erreur et on quitte
		ft_putendl_fd("minishell: error: failed to tokenize input", 2);
		return;
	}
	// Affichage de la liste de tokens pour debug
	print_token_list(tokens);
	// TODO : parser_logique(tokens);
	// Libération de la liste de tokens
	free_token_list(tokens);
}
