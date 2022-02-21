/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ztune <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 0:01:01 by ztune              #+#    #+#             */
/*   Updated: 2022/02/08 0:02:02 by ztune             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;
	int		dif;

	i = 0;
	while ((((const unsigned char *)s1)[i] || ((const unsigned char *)s2)[i]))
	{
		dif = ((const unsigned char *)s1)[i] - ((const unsigned char *)s2)[i];
		if (dif != 0)
		{
			return (dif);
		}
		i++;
	}
	return (0);
}
