#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

# include <stdlib.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

typedef struct s_gnl_buffer
{
	char	*data;
	size_t	start;
	size_t	end;
	size_t	capacity;
} t_gnl_buffer;

typedef struct s_gnl_fd
{
	int				fd;
	t_gnl_buffer		buf;
	struct s_gnl_fd	*next;
} t_gnl_fd;

char	*get_next_line(int fd);
size_t	gnl_newline(const char *str, size_t len);
int		gnl_append(t_gnl_buffer *buf, const char *chunk, size_t len);
void	gnl_reset(t_gnl_buffer *buf);

#endif
