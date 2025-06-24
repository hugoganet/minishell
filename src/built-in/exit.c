/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hganet <hganet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 14:24:18 by bernard           #+#    #+#             */
/*   Updated: 2025/06/24 17:28:59 by hganet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Vérifie si une chaîne est un nombre valide pour le builtin exit
 *
 * @param str La chaîne à vérifier
 * @return int 1 si c'est un nombre valide, 0 sinon
 */
static int ft_is_valid_number(char *str)
{
	int i;

	i = 0;
	if (str == NULL || *str == '\0')
		return (0);
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (str[i] == '\0') // Si la chaîne est seulement "+" ou "-"
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

/**
 * @brief Implémentation du builtin exit
 *
 * @param data Structure shell contenant les données globales
 * @param node Noeud AST de la commande exit
 * @return `int` Code de sortie (ne retourne que si une erreur est rencontrée)
 */
int ft_exit(t_shell *data, t_ast *node)
{
	unsigned char status;

	ft_putstr_fd("exit\n", STDERR_FILENO);
	// Par défaut, utilise le dernier code de sortie
	status = data->last_exit_status;
	// Si au moins un argument est fourni
	if (node->args[1])
	{
		if (!ft_is_valid_number(node->args[1]))
		{
			ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
			ft_putstr_fd(node->args[1], STDERR_FILENO);
			ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
			cleanup_shell(data);
			exit(2); // Code d'erreur standard pour argument invalide
		}
		else if (node->args[2]) // Si plus d'un argument et le premier est valide
		{
			ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
			return (1); // On ne quitte pas, on retourne une erreur
		}
		else // Exactement un argument numérique
			status = (unsigned char)(ft_atoi(node->args[1]) % 256);
	}
	cleanup_shell(data);
	exit((int)status);
}
