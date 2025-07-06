/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 14:24:18 by bernard           #+#    #+#             */
/*   Updated: 2025/07/06 20:37:18 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <limits.h>

/**
 * @brief Vérifie si un chiffre causera un dépassement pour les nombres positifs
 *
 * @param result Résultat actuel
 * @param digit Chiffre à ajouter
 * @return int 1 si dépassement, 0 sinon
 */
static int will_overflow_positive(long long result, int digit)
{
	return (result > LLONG_MAX / 10 ||
			(result == LLONG_MAX / 10 && digit > LLONG_MAX % 10));
}

/**
 * @brief Vérifie si un chiffre causera un dépassement pour les nombres négatifs
 *
 * @param result Résultat actuel
 * @param digit Chiffre à ajouter
 * @return int 1 si dépassement, 0 sinon
 */
static int will_overflow_negative(long long result, int digit)
{
	return (result > 922337203685477580LL ||
			(result == 922337203685477580LL && digit > 8));
}

/**
 * @brief Parse le signe et retourne l'index après le signe
 *
 * @param str La chaîne à parser
 * @param sign Pointeur vers la variable de signe
 * @return int Index après le signe
 */
static int parse_sign(char *str, int *sign)
{
	int i;

	i = 0;
	*sign = 1;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			*sign = -1;
		i++;
	}
	return (i);
}

/**
 * @brief Vérifie si une chaîne est un nombre valide et dans les limites LLONG
 *
 * @param str La chaîne à vérifier
 * @return int 1 si c'est un nombre valide et dans les limites, 0 sinon
 */
static int ft_is_valid_number(char *str)
{
	int i;
	int sign;
	long long result;
	int digit;

	if (str == NULL || *str == '\0')
		return (0);
	i = parse_sign(str, &sign);
	if (str[i] == '\0')
		return (0);
	result = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		digit = str[i] - '0';
		if ((sign == 1 && will_overflow_positive(result, digit)) ||
			(sign == -1 && will_overflow_negative(result, digit)))
			return (0);
		result = result * 10 + digit;
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
