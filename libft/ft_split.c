/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ztune <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 0:01:01 by ztune              #+#    #+#             */
/*   Updated: 2022/02/08 0:02:02 by ztune             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_split_count(char const *str, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		while (str[i] && str[i] == c)
		{
			i++;
		}
		if (str[i])
		{
			count++;
		}
		while (str[i] && str[i] != c)
		{
			i++;
		}
	}
	return (count);
}

static int	ft_split_next_del(char const *str, char c)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != c)
	{
		i++;
	}
	return (i);
}

static char	*ft_split_newmalloc(size_t size)
{
	char	*ret;

	ret = malloc(sizeof(char) * size + 1);
	if (!ret)
	{
		return (NULL);
	}
	ft_memset(ret, '\0', size + 1);
	return (ret);
}

static char	**ft_split_dbl(char const *s, char c, int word_i, int s_char_i)
{
	int		split_count;
	int		word_length;
	char	**ret;

	split_count = ft_split_count(s, c);
	ret = malloc(sizeof(char *) * (split_count + 1));
	if (!ret)
		return (NULL);
	while (word_i < split_count)
	{
		while (s[s_char_i] == c)
			s_char_i++;
		word_length = ft_split_next_del(&s[s_char_i], c);
		ret[word_i] = ft_split_newmalloc(word_length + 1);
		if (!ret[word_i])
		{
			ft_arrayfree((void ***)&ret, word_i);
			return (NULL);
		}
		ft_memcpy(ret[word_i], s + s_char_i, word_length);
		s_char_i = s_char_i + word_length;
		word_i++;
	}
	ret[word_i] = NULL;
	return (ret);
}

char	**ft_split(char const *s, char c)
{
	char	**ret;

	if (s == NULL)
	{
		return (NULL);
	}
	ret = ft_split_dbl(s, c, 0, 0);
	return (ret);
}
