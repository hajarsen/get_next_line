
char *ft_strchr(const char *str, int n)
{
    char *tmp;
    if (!str)
        return (NULL);
    tmp = (char *)str;
    while (*tmp != (char)n)
    {
        if (*tmp == 0)  
            return (NULL);
        tmp++;
    }
    return ((char *)tmp);
}


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


char *ft_strdup(const char *str)
{
    size_t i;
    size_t len;
    char *s;
    if (!str)
        return (NULL);
    len = ft_strlen(str) + 1; 
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


char *ft_substr(char const *s, unsigned int start, size_t len)
{
    size_t i;
    size_t srclen;
    char *str;
    if (!s)
        return (NULL);
    srclen = ft_strlen(s);
    if (start >= srclen)  
        return (ft_strdup(""));
    if (start + len > srclen)  
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


char *ft_strjoin(char const *s1, char const *s2)
{
    char *str;
    size_t i;
    size_t j;
    size_t size;
    if (!s1 || !s2)
        return (NULL);
    size = ft_strlen(s1) + ft_strlen(s2) + 1;  
    str = (char *)malloc(sizeof(char) * size);
    if (!str)
        return (NULL);
    i = 0;
    while (s1[i])  
    {
        str[i] = s1[i];
        i++;
    }
    j = 0;
    while (s2[j])  
    {
        str[i + j] = s2[j];
        j++;
    }
    str[i + j] = '\0';
    return (str);
}
