/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 20:18:24 by jinzhang          #+#    #+#             */
/*   Updated: 2025/06/01 12:04:30 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

int	gnl_strlen(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
		i++;
	return (i);
}

int	gnl_find_nl(char *buf, char c)
{
	int	i;

	i = 0;
	while (buf[i])
	{
		if (buf[i] == c)
			return (i);
		i++;
	}
	return (-1);
}

char	*gnl_error_eof(t_gnl *gnl, char *buf)
{
	if (*buf)
	{
		buf[0] = '\0';
		return (gnl->line);
	}
	free(gnl->line);
	gnl->line = NULL;
	return (NULL);
}

int	gnl_updatebuf(char *buf)
{
	int		i;
	t_gnl	gnl;

	i = 0;
	gnl.nl = gnl_find_nl(buf, '\n');
	if (gnl.nl != -1 && (buf[gnl.nl + 1] || buf[gnl.nl]))
	{
		while (buf[i])
		{
			if ((gnl.nl + i + 1) <= gnl_strlen(buf))
				buf[i] = buf[gnl.nl + i + 1];
			i++;
		}
		buf[i] = '\0';
	}
	return (gnl.nl);
}

char	*gnl_strjoin(char *pre_line, char *buf, int nl)
{
	int		i;
	int		j;
	char	*str;

	i = gnl_strlen(pre_line);
	if (nl == -1)
		nl = gnl_strlen(buf);
	else
		nl += 1;
	str = malloc((i + nl + 1) * sizeof(char));
	if (!str)
	{
		free(pre_line);
		return (NULL);
	}
	j = -1;
	while (++j < i)
		str[j] = pre_line[j];
	j = 0;
	while (j < nl)
		str[i++] = buf[j++];
	str[i] = '\0';
	free(pre_line);
	pre_line = NULL;
	return (str);
}
