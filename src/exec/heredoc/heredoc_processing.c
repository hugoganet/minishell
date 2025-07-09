/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_processing.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 00:00:00 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/09 15:55:53 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"
#include <signal.h>

/**
 * @brief Traite une ligne d'entrée de l'heredoc.
 * 
 * Cette fonction vérifie si la ligne est le délimiteur,
 * gère les signaux, étend la ligne si nécessaire,
 * et écrit la ligne dans le pipe de l'heredoc.
 * 
 * @param line La ligne d'entrée de l'utilisateur.
 * @param delimiter_clean Le délimiteur de l'heredoc sans quotes.
 * @param shell La structure principale du shell.
 * @param pipefd Le descripteur de fichier du pipe de l'heredoc.
 * @return 1 si la ligne a été écrite, 0 si c'est le délimiteur, -1 en cas d'erreur.
 */
static int	process_heredoc_input_line(char *line, char *delimiter_clean,
		t_shell *shell, int pipefd)
{
	if (!line)
		return (0);
	if (is_delimiter_line(line, delimiter_clean))
	{
		free(line);
		return (0);
	}
	if (g_signal == SIGINT)
	{
		free(line);
		return (-1);
	}
	// On traite la ligne d'entrée de l'heredoc
	// on test l'expansion de la ligne uniquement si le délimiteur n'est pas
	// entre quotes, sinon on laisse la ligne telle quelle.
	line = expand_heredoc_line(line, !is_heredoc_delimiter_quoted(
				delimiter_clean), shell);
	if (line)
	{
		write(pipefd, line, ft_strlen(line));
		write(pipefd, "\n", 1);
		free(line);
	}
	return (1);
}

/**
 * @brief Lit les lignes d'un heredoc jusqu'à ce que le délimiteur soit atteint
 * ou que l'utilisateur interrompe avec Ctrl+C.
 */
static int	read_heredoc_lines(char *delimiter_clean, t_shell *shell,
		int pipefd)
{
	char	*line;
	int		result;

	// Boucle principale pour lire les lignes de l'heredoc
	while (1)
	{
		// prompt du heredoc
		printf("> ");
		fflush(stdout);
		// On utilise fflush(stdout) pour s'assurer que le prompt est affiché
		// On récupère la ligne d'entrée de l'utilisateur
		// get_next_line lit une ligne depuis l'entrée standard (STDIN_FILENO)
		line = get_next_line(STDIN_FILENO);
		// Si l'utilisateur a appuyé sur Ctrl+C, g_signal est mis à SIGINT
		if (g_signal == SIGINT)
		{
			// On libère la ligne lue et on affiche une nouvelle ligne
			if (line)
				free(line);
			printf("\n");
			return (0);
		}
		// Si la ligne est vide (fin de l'entrée), on affiche une nouvelle ligne
		// et on retourne 0 pour indiquer la fin de l'heredoc
		if (!line)
		{
			printf("\n");
			return (-1);
		}
		result = process_heredoc_input_line(line, delimiter_clean,
				shell, pipefd);
		if (result <= 0)
			return (result == 0);
	}
}

/**
 * @brief Fonction principale pour traiter un heredoc.
 */
int	process_heredoc_main(t_shell *shell,
		int pipefd[2], char *delimiter_clean)
{
	int	read_result;

	read_result = read_heredoc_lines(delimiter_clean, shell, pipefd[1]);
	// Quand le heredoc est terminé, on ferme le descripteur d'écriture du pipe
	close(pipefd[1]);
	// Si l'utilisateur a appuyé sur Ctrl+C, on ferme le pipe et on retourne 130
	if (read_result == 0)
	{
		close(pipefd[0]);
		g_signal = 0;
		return (130);
	}
	// ajoute le descripteur de fichier du pipe de l'heredoc à la liste des heredoc_fds
	// c'est indispensable pour pouvoir le fermer plus tard si on gère plusieurs heredocs
	add_heredoc_fd(shell, pipefd[0]);
	return (0);
}
