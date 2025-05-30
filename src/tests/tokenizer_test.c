/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_test.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 13:52:10 by hugoganet         #+#    #+#             */
/*   Updated: 2025/05/29 13:54:29 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Lance un test de tokenisation et affiche les résultats.
 *
 * @param input Ligne de commande à tester.
 */
static void run_token_test(const char *input)
{
	t_token *tokens;

	printf("==> Test : \"%s\"\n", input);
	tokens = tokenize((char *)input);
	if (!tokens)
	{
		printf("❌ Erreur de tokenisation\n\n");
		return;
	}
	print_token_list(tokens);
	free_token_list(tokens);
	printf("\n");
}

/**
 * @brief Fonction principale de test pour la tokenisation.
 *
 * Cette fonction exécute plusieurs cas limites pour valider la
 * robustesse de la fonction `tokenize()`. Aucun parsing ou exec n’est appelé.
 */
int main(void)
{
	run_token_test("ls");
	run_token_test("echo \"bonjour\"");
	run_token_test("echo 'test'");
	run_token_test("echo bonjour > file.txt");
	run_token_test("cat < infile | grep toto > outfile");
	run_token_test("echo \"bonjour"); // Erreur : quotes non fermées
	run_token_test("ls ||| grep");	  // Erreur syntaxique
	run_token_test(">");
	run_token_test("ls | wc -l");
	run_token_test("echo \"$HOME\"");
	run_token_test("echo '$HOME'");
	return (0);
}