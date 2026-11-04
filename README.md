*This project has been created as part of the 42 curriculum by hsennane.*

# get_next_line

## Description

**get_next_line** is a C project that implements a function for reading a file descriptor **one line at a time**. It introduces low-level file I/O with `read()`, persistent state between function calls, dynamic memory allocation, and string manipulation.

Each call to `get_next_line(int fd)` returns the next available line, including the newline character when one is present. It returns `NULL` at end-of-file or on failure. The caller is responsible for freeing the returned string.

## Instructions

### Compilation

The repository contains separate mandatory and bonus implementations. Compile either version with a C compiler; no Makefile is required.

**Mandatory:**

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 \
  main.c get_next_line.c get_next_line_utils.c -o gnl
```

**Bonus:**

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 \
  main.c get_next_line_bonus.c get_next_line_utils_bonus.c -o gnl
```

`BUFFER_SIZE` controls how many bytes are requested in each `read()` call, not the maximum length of a line.

### Using get_next_line

A minimal example of reading a text file:

```c
#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int     fd;
    char    *line;

    fd = open("example.txt", O_RDONLY);
    if (fd < 0)
        return (1);
    while ((line = get_next_line(fd)) != NULL)
    {
        printf("%s", line);
        free(line);
    }
    close(fd);
    return (0);
}
```

The example is for the mandatory version. To use the bonus version, include `get_next_line_bonus.h` instead.

### Mandatory and Bonus

- **Mandatory:** Reads successive lines and retains unread data in a static array indexed by file descriptor, bounded by `MAX_FD`.
- **Bonus:** Uses a linked list to associate buffered data with each active file descriptor. Its buffer grows as needed rather than relying on a fixed descriptor-indexed array.

The original `get_next_line_utilis.c` is preserved, while `get_next_line_utils.c` contains the same functions under the standard filename expected by external testers. **Compile only one of these two utility files**, as they define identical symbols.

## Resources

### File Descriptors and read()

A **file descriptor** is an integer used by the operating system to identify an open file, pipe, or other input source. The `read()` system call attempts to retrieve up to a specified number of bytes.

| `read()` result | Meaning |
|---|---|
| Positive value | Number of bytes successfully read |
| `0` | End-of-file |
| `-1` | Read error |

A single `read()` call does not necessarily return a complete line. The function must accumulate data until it encounters a newline or reaches the end of the input.

### Buffering and Line Boundaries

Consider a file containing `hello\nworld`, read with `BUFFER_SIZE=4`:

```text
File contents:     hello\nworld
First read:        "hell"
Second read:       "o\nwo"
First line:        "hello\n"
Unread remainder:  "wo"
Next read:         "rld"
Second line:       "world"
```

The unread remainder must be preserved for the next call. The newline belongs to the returned line; a final line without `\n` is still returned.

### Persistent State

A local variable disappears when a function returns, but `get_next_line()` needs to remember bytes that have been read but not yet returned. **Static storage** makes that state available across calls.

The mandatory implementation stores a buffer for each permitted descriptor. The bonus version uses dynamically allocated nodes to track active descriptors and their remaining content.

### Memory Management

Every non-`NULL` line is allocated on the heap and must be released by the caller with `free()`. Internal buffers also require cleanup when reading finishes or an error occurs. This separation of ownership prevents a returned line from being overwritten by subsequent calls.

### Implementation Notes

The original mandatory implementation builds longer strings through repeated concatenation. With a small `BUFFER_SIZE`, that means copying previously accumulated data again and again, which can be slow for very long lines. The bonus implementation instead uses a growing buffer to reduce unnecessary copying.

### References

- [`read(2)` — Linux manual](https://man7.org/linux/man-pages/man2/read.2.html)
- [`open(2)` — Linux manual](https://man7.org/linux/man-pages/man2/open.2.html)
- [`close(2)` — Linux manual](https://man7.org/linux/man-pages/man2/close.2.html)
- [C dynamic memory management — cppreference](https://en.cppreference.com/w/c/memory)
