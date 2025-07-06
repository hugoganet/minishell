/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 14:18:46 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/06 14:31:08 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <termios.h>

volatile sig_atomic_t g_signal = 0;

// /**
//  * @brief Désactive l'affichage des caractères de contrôle (comme ^C).
//  *
//  * Cette fonction utilise les appels POSIX `tcgetattr` et `tcsetattr` pour modifier
//  * les attributs du terminal. En retirant le flag `ECHOCTL`, on empêche l'affichage
//  * de symboles comme `^C` quand l'utilisateur tape `Ctrl-C` ou `Ctrl-\`.
//  */
// static void set_echoctl_off(void)
// {
// 	// Structure pour les attributs du terminal
// 	struct termios term;

// 	// Récupère les attributs actuels du terminal pour STDIN (entrée standard).
// 	// Le résultat est stocké dans la structure `term`.
// 	// Si tcgetattr échoue (renvoie -1), on quitte la fonction.
// 	if (tcgetattr(STDIN_FILENO, &term) == -1)
// 		return;
// 	// Supprime le flag ECHOCTL (Echo Control Characters) dans les flags du terminal.
// 	// ECHOCTL est responsable de l'affichage visuel des caractères de contrôle.
// 	// Par exemple : Ctrl-C → ^C, Ctrl-\ → ^\.
// 	// Ici, on fait un "bitwise AND NOT" ( &= ~ ) pour désactiver juste ce flag
// 	// sans toucher aux autres (ECHO, ICANON, etc.).
// 	term.c_lflag &= ~ECHOCTL;
// 	// Applique les nouveaux paramètres immédiatement (sans attente de prochain flush).
// 	// TCSANOW signifie : "Change la configuration du terminal dès maintenant".
// 	// Cette ligne rend les modifications actives dans le terminal interactif du shell.
// 	tcsetattr(STDIN_FILENO, TCSANOW, &term);
// }
/**
 * @brief Handler pour SIGINT (Ctrl-C).
 *
 * Affiche une nouvelle ligne et marque le signal reçu.
 */
static void handle_sigint(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}
/**
 * @brief Active les handlers pour le shell interactif (readline).
 */
void init_signals(void)
{
	signal(SIGINT, handle_sigint);
	// On ignore SIGQUIT dans le shell interactif
	signal(SIGQUIT, SIG_IGN);
	// Désactive l'affichage des caractères de contrôle (comme ^C)
	// on ignore le signal SIGPIPE pour éviter les erreurs de pipe cassé
	// (par exemple, si un processus enfant est terminé avant que le parent
	// n'ait fini d'écrire dans le pipe ex : `sleep 1234 | ls`)
	signal(SIGPIPE, SIG_IGN);
	// set_echoctl_off();
}
