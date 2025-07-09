/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_core.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 15:00:00 by hugoganet         #+#    #+#             */
/*   Updated: 2025/07/09 18:34:09 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"
#include <signal.h>

/**
 * @brief Initialise les signaux pour la gestion des heredocs dans la
 * structure heredoc_fd.
 * 
 * @param shell La structure principale du shell contenant les informations
 * d'exécution.
 */
void	setup_heredoc_redirection(t_shell *shell)
{
	t_heredoc_fd	*last_heredoc;
	t_heredoc_fd	*current;

	last_heredoc = NULL;
	current = shell->heredoc_fds;
	while (current)
	{
		if (current->fd != -1)
			last_heredoc = current;
		current = current->next;
	}
	if (last_heredoc && last_heredoc->fd != -1)
	{
		if (dup2(last_heredoc->fd, STDIN_FILENO) == -1)
			perror("minishell: dup2 heredoc");
		close(last_heredoc->fd);
		last_heredoc->fd = -1;
	}
}

/**
 * @brief Traite les heredocs dans l'AST.
 * 
 * Parcourt l'AST à la recherche de nœuds de type HEREDOC,
 * et appelle `handle_heredoc` pour chaque nœud trouvé.
 * 
 * Si l'un des herdocs échoue avec un signal d'interruption (130),
 * ferme tous les descripteurs de fichiers d'heredoc ouverts
 * et libère les ressources associées avant de retourner 130.
 * 
 * @param ast_root Le nœud racine de l'AST à traiter.
 * @param shell La structure principale du shell contenant les informations
 * d'exécution.
 * @return `0` si tous les heredocs ont été traités avec succès,
 * 	   ou `130` si un heredoc a été interrompu par l'utilisateur.
 */
int	process_heredocs(t_ast *ast_root, t_shell *shell)
{
	t_ast	*tmp;
	int		heredoc_status;

	tmp = ast_root;
	while (tmp)
	{
		if (tmp->type == HEREDOC)
		{
			heredoc_status = handle_heredoc(tmp->str, shell);
			if (heredoc_status == 130)
			{
				close_all_heredoc_fds(shell);
				free_all_heredoc_fds(shell);
				return (130);
			}
		}
		tmp = tmp->right;
	}
	return (0);
}

/**
 * @brief Gère un heredoc en initialisant les signaux créant un
 * pipe, en validant le token, et en traitant l'entrée utilisateur
 * jusqu'à ce que le délimiteur soit atteint.
 * 
 * @param token_str La chaîne de caractères du token HEREDOC.
 * @param shell La structure principale du shell contenant les
 * informations d'exécution.
 * @return Le statut de l'exécution : 
 */
int	handle_heredoc(char *token_str, t_shell *shell)
{
	int					pipefd[2];
	char				*delimiter_clean;
	struct sigaction	sa_old;
	int					result;

	set_heredoc_sigint(&sa_old);
	if (init_heredoc_pipe(pipefd) != 0
		|| validate_heredoc_token(token_str, pipefd) != 0)
		return (restore_sigint(&sa_old), 1);
	delimiter_clean = expand_and_clean_delimiter(token_str + 2, shell);
	if (!delimiter_clean)
		return (close_pipe_fds(pipefd), restore_sigint(&sa_old), 1);
	result = process_heredoc_main(shell, pipefd, delimiter_clean);
	free(delimiter_clean);
	restore_sigint(&sa_old);
	return (result);
}
