#include "get_next_line_bonus.h"

void	gnl_reset(t_gnl_buffer *buf)
{
	free(buf->data);
	buf->data = NULL;
	buf->start = 0;
	buf->end = 0;
	buf->capacity = 0;
}

size_t	gnl_newline(const char *str, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
	{
		if (str[i] == '\n')
			return (i + 1);
		i++;
	}
	return (0);
}

static void	gnl_replace(t_gnl_buffer *buf, char *data, size_t used)
{
	size_t	i;

	i = 0;
	while (i < used)
	{
		data[i] = buf->data[buf->start + i];
		i++;
	}
	free(buf->data);
	buf->data = data;
	buf->start = 0;
	buf->end = used;
}

static int	gnl_grow(t_gnl_buffer *buf, size_t count)
{
	size_t	used;
	size_t	cap;
	char	*data;

	used = buf->end - buf->start;
	if (count >= (size_t)-1 - used)
		return (0);
	if (buf->data && count < buf->capacity - buf->end)
		return (1);
	cap = buf->capacity;
	if (cap < 64)
		cap = 64;
	while (cap < used + count + 1 && cap <= (size_t)-1 / 2)
		cap *= 2;
	if (cap < used + count + 1)
		cap = used + count + 1;
	data = malloc(cap);
	if (!data)
		return (0);
	gnl_replace(buf, data, used);
	buf->capacity = cap;
	return (1);
}

int	gnl_append(t_gnl_buffer *buf, const char *chunk, size_t len)
{
	size_t	i;

	if (!gnl_grow(buf, len))
		return (0);
	i = 0;
	while (i < len)
	{
		buf->data[buf->end + i] = chunk[i];
		i++;
	}
	buf->end += len;
	buf->data[buf->end] = '\0';
	return (1);
}
