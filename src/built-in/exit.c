/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 14:24:18 by bernard           #+#    #+#             */
/*   Updated: 2025/07/06 16:29:50 by hugoganet        ###   ########.fr       */
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
	if (str[i] == '\0')
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
 * @brief Affiche le message d'erreur pour argument non numérique et quitte
 *
 * @param data Structure shell contenant les données globales
 * @param arg L'argument invalide
 */
static void exit_with_numeric_error(t_shell *data, char *arg)
{
	ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
	cleanup_shell(data);
	exit(2);
}

/**
 * @brief Traite les arguments de la commande exit
 *
 * @param node Noeud AST de la commande exit
 * @param data Structure shell contenant les données globales
 * @return Code de sortie ou -1 pour utiliser le dernier exit status
 */
static int process_exit_args(t_ast *node, t_shell *data)
{
	if (!node->args[1])
		return (-1);
	if (node->args[1][0] == '\0')
		exit_with_numeric_error(data, node->args[1]);
	if (!ft_is_valid_number(node->args[1]))
		exit_with_numeric_error(data, node->args[1]);
	if (node->args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
		return (1);
	}
	return ((unsigned char)(ft_atoi(node->args[1]) % 256));
}

/**
 * @brief Implémentation du builtin exit
 *
 * @param data Structure shell contenant les données globales
 * @param node Noeud AST de la commande exit
 * @return `int` Code de sortie (ne retourne que si une erreur est rencontrée)
 */
int ft_exit(t_ast *node, t_shell *data)
{
	unsigned char status;
	int result;

	ft_putstr_fd("exit\n", STDERR_FILENO);
	status = data->last_exit_status;
	result = process_exit_args(node, data);
	if (result == -1)
		status = data->last_exit_status;
	else if (result == 1)
		return (1);
	else
		status = (unsigned char)result;
	cleanup_shell(data);
	exit((int)status);
}
