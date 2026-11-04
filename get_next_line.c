/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsennane <hajarsennane1@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 17:12:44 by hsennane          #+#    #+#             */
/*   Updated: 2026/11/04 01:30:00 by hsennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static void	free_null(char **ptr)
{
	if (ptr && *ptr)
	{
		free(*ptr);
		*ptr = NULL;
	}
}

static char	*join_line(int pos, char **buffer)
{
	char	*res;
	char	*tmp;

	if (pos <= 0)
	{
		if (!*buffer || **buffer == '\0')
		{
			free_null(buffer);
			return (NULL);
		}
		res = *buffer;
		*buffer = NULL;
		return (res);
	}
	tmp = ft_substr(*buffer, pos, ft_strlen(*buffer) - pos);
	res = ft_substr(*buffer, 0, pos);
	free_null(buffer);
	*buffer = tmp;
	return (res);
}

static int	read_chunk(int fd, char **buffer, char *chunk)
{
	ssize_t	bytes_read;
	char	*tmp;

	bytes_read = read(fd, chunk, BUFFER_SIZE);
	if (bytes_read <= 0)
		return ((int)bytes_read);
	chunk[bytes_read] = '\0';
	tmp = ft_strjoin(*buffer, chunk);
	free_null(buffer);
	if (!tmp)
		return (-1);
	*buffer = tmp;
	return (1);
}

static char	*read_line(int fd, char **buffer, char *chunk)
{
	char	*nl;
	int		status;

	nl = ft_strchr(*buffer, '\n');
	while (!nl)
	{
		status = read_chunk(fd, buffer, chunk);
		if (status < 0)
		{
			free_null(buffer);
			return (NULL);
		}
		if (status == 0)
			return (join_line(0, buffer));
		nl = ft_strchr(*buffer, '\n');
	}
	return (join_line(nl - *buffer + 1, buffer));
}

char	*get_next_line(int fd)
{
	static char	*buffer[MAX_FD + 1];
	char		*chunk;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0 || fd > MAX_FD)
		return (NULL);
	chunk = malloc((size_t)BUFFER_SIZE + 1);
	if (!chunk)
	{
		free_null(&buffer[fd]);
		return (NULL);
	}
	if (!buffer[fd])
		buffer[fd] = ft_strdup("");
	if (!buffer[fd])
	{
		free(chunk);
		return (NULL);
	}
	line = read_line(fd, &buffer[fd], chunk);
	free(chunk);
	return (line);
}
