#include "get_next_line_bonus.h"

static char	*gnl_take_line(t_gnl_buffer *buf)
{
	size_t	n;
	size_t	i;
	char	*line;

	n = buf->end - buf->start;
	if (!n)
		return (NULL);
	i = gnl_newline(buf->data + buf->start, n);
	if (i)
		n = i;
	line = malloc(n + 1);
	if (!line)
		return (NULL);
	i = n;
	while (i-- > 0)
		line[i] = buf->data[buf->start + i];
	line[n] = '\0';
	buf->start += n;
	if (buf->start == buf->end)
		gnl_reset(buf);
	return (line);
}

static t_gnl_fd	*gnl_node(t_gnl_fd **head, int fd, int create)
{
	t_gnl_fd	*node;

	node = *head;
	while (node)
	{
		if (node->fd == fd)
			return (node);
		node = node->next;
	}
	if (!create)
		return (NULL);
	node = malloc(sizeof(t_gnl_fd));
	if (!node)
		return (NULL);
	node->fd = fd;
	node->buf.data = NULL;
	node->buf.start = 0;
	node->buf.end = 0;
	node->buf.capacity = 0;
	node->next = *head;
	*head = node;
	return (node);
}

static void	gnl_remove(t_gnl_fd **head, t_gnl_fd *node)
{
	t_gnl_fd	**slot;

	slot = head;
	while (*slot && *slot != node)
		slot = &(*slot)->next;
	if (*slot)
	{
		*slot = node->next;
		gnl_reset(&node->buf);
		free(node);
	}
}

static int	gnl_fill(int fd, t_gnl_buffer *buf)
{
	char	*chunk;
	ssize_t	bytes;

	chunk = malloc((size_t)BUFFER_SIZE);
	if (!chunk)
		return (0);
	bytes = 0;
	while (1)
	{
		bytes = read(fd, chunk, BUFFER_SIZE);
		if (bytes <= 0)
			break ;
		if (!gnl_append(buf, chunk, (size_t)bytes))
		{
			bytes = -1;
			break ;
		}
		if (gnl_newline(chunk, (size_t)bytes))
			break ;
	}
	free(chunk);
	return (bytes >= 0);
}

char	*get_next_line(int fd)
{
	static t_gnl_fd	*head;
	t_gnl_fd		*node;
	char			*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	node = gnl_node(&head, fd, 1);
	if (!node)
		return (NULL);
	if (read(fd, NULL, 0) < 0 || ((!node->buf.data ||
			!gnl_newline(node->buf.data + node->buf.start,
				node->buf.end - node->buf.start))
			&& !gnl_fill(fd, &node->buf)))
	{
		gnl_remove(&head, node);
		return (NULL);
	}
	line = gnl_take_line(&node->buf);
	if (!line || !node->buf.data)
		gnl_remove(&head, node);
	return (line);
}
