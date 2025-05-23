/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 17:52:43 by hugoganet         #+#    #+#             */
/*   Updated: 2025/05/23 17:56:56 by hugoganet        ###   ########.fr       */
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

	(void)shell; // utile plus tard
	tokens = tokenize(input); // Tokenisation de la ligne d'entrée
	if (!tokens) // Si la tokenisation échoue
	{
		ft_putendl_fd("minishell: error: failed to tokenize input", 2);
		return;
	}

	// TODO : parser_logique(tokens);
	// TODO : free_token_list(tokens);
}
