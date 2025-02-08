// Libère la mémoire pointée par ptr et met le pointeur à NULL
static void free_null(char **ptr)
{
    if (ptr && *ptr)
    {
        free(*ptr);
        *ptr = NULL;
    }
}

// Extrait la ligne jusqu'au prochain \n ou la fin du buffer
// nl_position: position du \n ou 0 si pas de \n
static char *join_line(int nl_position, char **buffer)
{
    char *res;
    char *tmp;
    if (nl_position <= 0)  // Pas de \n trouvé
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
    // Sépare la ligne trouvée du reste du buffer
    tmp = ft_substr(*buffer, nl_position, ft_strlen(*buffer) - nl_position);
    res = ft_substr(*buffer, 0, nl_position);
    free_null(buffer);
    *buffer = tmp;
    return (res);
}

// Lit depuis le fd jusqu'à trouver un \n ou la fin du fichier
static char *read_line(int fd, char **buffer, char *read_return)
{
    ssize_t bytes_read;
    char *tmp;
    char *nl;
    nl = ft_strchr(*buffer, '\n');
    while (nl == NULL)  // Continue de lire tant qu'on ne trouve pas de \n
    {
        bytes_read = read(fd, read_return, BUFFER_SIZE);
        if (bytes_read < 0)  // Erreur de lecture
        {
            free_null(buffer);
            return (NULL);
        }
        if (bytes_read == 0)  // Fin du fichier
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

// Fonction principale qui lit une ligne depuis un descripteur de fichier
char *get_next_line(int fd)
{
    static char *buffer[MAX_FD + 1];  // Buffer statique pour chaque fd
    char *read_return;
    char *res;
    
    // Vérifie les paramètres invalides
    if (fd < 0 || BUFFER_SIZE <= 0 || fd > MAX_FD)
        return (NULL);
    
    // Alloue le buffer temporaire pour la lecture
    read_return = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
    if (!read_return)
    {
        free_null(&buffer[fd]);
        return (NULL);
    }
    
    // Initialise le buffer si c'est le premier appel
    if (!buffer[fd])
    {
        buffer[fd] = ft_strdup("");
        if (!buffer[fd])
        {
            free_null(&read_return);
            return (NULL);
        }
    }
    
    // Lit la ligne et nettoie le buffer temporaire
    res = read_line(fd, &buffer[fd], read_return);
    free_null(&read_return);
    return (res);
}
