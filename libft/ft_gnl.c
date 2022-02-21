/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_gnl.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ztune <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 0:01:01 by ztune              #+#    #+#             */
/*   Updated: 2022/02/08 0:02:02 by ztune             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*gnl_strcpy_shift(char *dest, const char *src, int shift)
{
	int	i;

	i = 0;
	while (src[i + shift])
	{
		dest[i] = src[i + shift];
		i++;
	}
	dest[i] = src[i + shift];
	return (dest);
}

static int	gnl_strchr_length(char *s, int c)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == (char)c)
			return (i);
		i++;
	}
	if ((char)c == '\0')
		return (i);
	return (-1);
}

static char	*gnl_strjoin(char **s1, const char *s2)
{
	char	*ret;
	int		s1len;
	int		s2len;

	if (*s1 == NULL && s2 == NULL)
		return (ft_strdup(""));
	if (*s1 != NULL && s2 == NULL)
		return (ft_strdup(*s1));
	if (*s1 == NULL && s2 != NULL)
		return (ft_strdup(s2));
	s1len = ft_strlen(*s1);
	s2len = ft_strlen(s2);
	ret = malloc(sizeof(char) * (s1len + s2len + 1));
	if (ret == NULL)
	{
		free(*s1);
		*s1 = NULL;
		return (NULL);
	}
	ft_memcpy(ret, *s1, s1len);
	ft_memcpy(ret + s1len, s2, s2len);
	ret[s1len + s2len] = '\0';
	free(*s1);
	*s1 = NULL;
	return (ret);
}

static int	gnl_after(int fd, int *gnl_start, char *after, char **line)
{
	int	before;

	if (fd < 0 || GNL_BUFF_SIZE <= 0 || line == NULL)
		return (-2);
	*line = NULL;
	if (!*gnl_start && *gnl_start++)
		after[0] = '\0';
	if (after[0] == '\0')
		return (-1);
	before = gnl_strchr_length(after, '\n');
	if (before >= 0)
	{
		after[before] = '\0';
		*line = ft_strdup(after);
		gnl_strcpy_shift(after, after, before + 1);
	}
	if (before == -1)
	{
		*line = ft_strdup(after);
		after[0] = '\0';
	}
	if (*line == NULL)
		return (-2);
	return (before);
}

int	ft_gnl(int fd, char **line)
{
	static int	gnl_start;
	int			bytes;
	int			before;
	char		buf[GNL_BUFF_SIZE + 1];
	static char	after[GNL_BUFF_SIZE + 1];

	bytes = 1;
	before = gnl_after(fd, &gnl_start, after, line);
	while (bytes > 0 && before == -1)
	{
		bytes = read(fd, buf, GNL_BUFF_SIZE);
		if (bytes == -1)
			break ;
		buf[bytes] = '\0';
		before = gnl_strchr_length(buf, '\n');
		if (before >= 0 && gnl_strcpy_shift(after, buf, before + 1))
			buf[before] = '\0';
		*line = gnl_strjoin(line, buf);
	}
	if (before == -2 || bytes == -1 || (bytes > 0 && *line == NULL))
		return (-1);
	if (bytes == 0)
		return (0);
	return (1);
}
