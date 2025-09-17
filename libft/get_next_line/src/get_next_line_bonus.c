/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 20:18:29 by jinzhang          #+#    #+#             */
/*   Updated: 2025/06/01 16:02:19 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

#if !defined(BUFFER_SIZE) || BUFFER_SIZE <= -2
# undef BUFFER_SIZE
# define BUFFER_SIZE 0
#endif

char	*get_next_line(int fd)
{
	t_gnl		gnl;
	static char	buf[1024][BUFFER_SIZE + 1];

	if (fd < 0 || fd > 1023 || BUFFER_SIZE <= 0)
		return (NULL);
	gnl.line = malloc(1);
	if (!gnl.line)
		return (NULL);
	gnl.line[0] = '\0';
	gnl.nl = gnl_updatebuf(buf[fd]);
	while (1)
	{
		if (gnl.nl == -1)
		{
			gnl.bytes_read = read(fd, buf[fd], BUFFER_SIZE);
			if (gnl.bytes_read <= 0)
				return (gnl_error_eof(&gnl, buf[fd]));
			buf[fd][gnl.bytes_read] = '\0';
		}
		gnl.nl = gnl_find_nl(buf[fd], '\n');
		gnl.line = gnl_strjoin(gnl.line, buf[fd], gnl.nl);
		if (gnl.nl != -1 || gnl.line == NULL)
			break ;
	}
	return (gnl.line);
}
