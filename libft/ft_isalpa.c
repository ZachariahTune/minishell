/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbell <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/10 12:37:34 by rbell             #+#    #+#             */
/*   Updated: 2021/10/10 12:37:57 by rbell            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isalpha(int c)
{
	return ((c > 64 && c < 91) || (c > 96 && c < 123));
}

int	ft_isalnum(int c)
{
	return ((c > 64 && c < 91) || (c > 96 && c < 123) || (c > 47 && c < 58));
}

int	ft_is_low_dash(int c)
{
	return (c == 95);
}

int	ft_is_var_name(int c)
{
	return (ft_isalnum(c) || ft_is_low_dash(c));
}

int	ft_is_begin_var(int c)
{
	return (ft_isalpha(c) || ft_is_low_dash(c));
}
