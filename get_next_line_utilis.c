// Cherche la première occurrence du caractère n dans la chaîne str
// Retourne un pointeur vers ce caractère ou NULL si non trouvé
char *ft_strchr(const char *str, int n)
{
    char *tmp;
    if (!str)
        return (NULL);
    tmp = (char *)str;
    while (*tmp != (char)n)
    {
        if (*tmp == 0)  // Si on atteint la fin de la chaîne sans trouver n
            return (NULL);
        tmp++;
    }
    return ((char *)tmp);
}

// Calcule la longueur d'une chaîne de caractères
// Retourne 0 si la chaîne est NULL
size_t ft_strlen(const char *str)
{
    size_t a;
    if (!str)
        return (0);
    a = 0;
    while (str[a])
        a++;
    return (a);
}

// Crée une copie dynamique d'une chaîne de caractères
// La nouvelle chaîne est allouée sur le tas (heap)
char *ft_strdup(const char *str)
{
    size_t i;
    size_t len;
    char *s;
    if (!str)
        return (NULL);
    len = ft_strlen(str) + 1;  // +1 pour le caractère nul final
    s = (char *)malloc(sizeof(char) * len);
    if (!s)
        return (NULL);
    i = 0;
    while (i < len - 1)
    {
        s[i] = str[i];
        i++;
    }
    s[i] = '\0';
    return (s);
}

// Extrait une sous-chaîne d'une chaîne source
// start: index de début, len: longueur à extraire
char *ft_substr(char const *s, unsigned int start, size_t len)
{
    size_t i;
    size_t srclen;
    char *str;
    if (!s)
        return (NULL);
    srclen = ft_strlen(s);
    if (start >= srclen)  // Si start dépasse la longueur de la chaîne
        return (ft_strdup(""));
    if (start + len > srclen)  // Ajuste len si elle dépasse la fin de la chaîne
        len = srclen - start;
    str = (char *)malloc(sizeof(char) * (len + 1));
    if (!str)
        return (NULL);
    i = 0;
    while (s[start + i] && i < len)
    {
        str[i] = s[start + i];
        i++;
    }
    str[i] = '\0';
    return (str);
}

// Concatène deux chaînes de caractères dans une nouvelle chaîne
char *ft_strjoin(char const *s1, char const *s2)
{
    char *str;
    size_t i;
    size_t j;
    size_t size;
    if (!s1 || !s2)
        return (NULL);
    size = ft_strlen(s1) + ft_strlen(s2) + 1;  // +1 pour le caractère nul
    str = (char *)malloc(sizeof(char) * size);
    if (!str)
        return (NULL);
    i = 0;
    while (s1[i])  // Copie la première chaîne
    {
        str[i] = s1[i];
        i++;
    }
    j = 0;
    while (s2[j])  // Ajoute la deuxième chaîne
    {
        str[i + j] = s2[j];
        j++;
    }
    str[i + j] = '\0';
    return (str);
}
