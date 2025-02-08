#include "get_next_line.h"

static void free_null(char **ptr)
{
    if (ptr && *ptr)
    {
        free(*ptr);
        *ptr = NULL;
    }
}


static char *join_line(int nl_position, char **buffer)
{
    char *res;
    char *tmp;
    if (nl_position <= 0)  
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
   
    tmp = ft_substr(*buffer, nl_position, ft_strlen(*buffer) - nl_position);
    res = ft_substr(*buffer, 0, nl_position);
    free_null(buffer);
    *buffer = tmp;
    return (res);
}


static char *read_line(int fd, char **buffer, char *read_return)
{
    ssize_t bytes_read;
    char *tmp;
    char *nl;
    nl = ft_strchr(*buffer, '\n');
    while (nl == NULL)  
    {
        bytes_read = read(fd, read_return, BUFFER_SIZE);
        if (bytes_read < 0)  
        {
            free_null(buffer);
            return (NULL);
        }
        if (bytes_read == 0)  
            return (join_line(bytes_read, buffer));
        read_return[bytes_read] = '\0';
        tmp = ft_strjoin(*buffer, read_return);
        free_null(buffer);
        if (!tmp)
            return (NULL);
        *buffer = tmp;
        nl = ft_strchr(*buffer, '\n');
    }
    return (join_line(nl - *buffer + 1, buffer));
}


char *get_next_line(int fd)
{
    static char *buffer[MAX_FD + 1];  
    char *read_return;
    char *res;
    

    if (fd < 0 || BUFFER_SIZE <= 0 || fd > MAX_FD)
        return (NULL);
    

    read_return = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
    if (!read_return)
    {
        free_null(&buffer[fd]);
        return (NULL);
    }
    
    if (!buffer[fd])
    {
        buffer[fd] = ft_strdup("");
        if (!buffer[fd])
        {
            free_null(&read_return);
            return (NULL);
        }
    }
    
    res = read_line(fd, &buffer[fd], read_return);
    free_null(&read_return);
    return (res);
}
