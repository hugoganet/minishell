/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elaudrez <elaudrez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 13:46:40 by elaudrez          #+#    #+#             */
/*   Updated: 2025/07/09 18:14:37 by elaudrez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Libère le tableau d'arguments en cas d'erreur d'allocation.
 *
 * @param args Tableau d'arguments à libérer
 * @param index Index jusqu'auquel libérer les éléments
 */
void	free_args_on_error(char **args, int index)
{
	while (--index >= 0)
		free(args[index]);
	free(args);
}

/**
 * @brief Parse le signe et retourne l'index après le signe
 *
 * @param str La chaîne à parser
 * @param sign Pointeur vers la variable de signe
 * @return int Index après le signe
 */
int	parse_sign(char *str, int *sign)
{
	int	i;

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
 * @brief Vérifie si un chiffre causera un dépassement pour les nombres
 * négatifs
 *
 * @param result Résultat actuel
 * @param digit Chiffre à ajouter
 * @return int 1 si dépassement, 0 sinon
 */
int	will_overflow_negative(long long result, int digit)
{
	return (result > 922337203685477580LL
		|| (result == 922337203685477580LL && digit > 8));
}

int	handle_invalid_export(char *arg)
{
	ft_putstr_fd("Minishell: export: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(" not a valid identifier\n", 2);
	return (1);
}
