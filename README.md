*This project has been created as part of the 42 curriculum by hsennane.*

# get_next_line

## Description

**get_next_line** is a C project that implements a function to read a file descriptor **one line at a time**. It covers low-level input with `read()`, dynamic memory allocation, string manipulation, and preserving unread data between function calls.

`get_next_line(int fd)` returns the next line, including its terminating newline when present. It returns `NULL` at end-of-file or on error. **The caller must free each returned line.**

## Instructions

### Compilation

Compile the mandatory or bonus implementation with a C compiler. The repository does not require a Makefile.

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

These commands assume you have created a `main.c` that calls `get_next_line()`. Use `get_next_line_bonus.h` with the bonus version.

### Usage Example

The following program reads a text file until the end of input:

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

### Mandatory and Bonus

- **Mandatory:** The original implementation maintains persistent buffers in a static array indexed by file descriptor. Its supported descriptor range is limited by `MAX_FD` (256 by default).
- **Bonus:** A linked list maintains an independent state for each file descriptor, using dynamically growing buffers to preserve unread input.

The original `get_next_line_utilis.c` remains in the repository. `get_next_line_utils.c` is a byte-identical copy under the filename expected by common 42 testers. Compile **only one** of the two utilities files, since both define the same functions.

## Resources

### File Descriptors and `read()`

A **file descriptor** identifies an open input source, such as a file, pipe, or standard input. The `read()` system call reads **up to** a requested number of bytes; it does not guarantee a complete line in one call.

| Return value | Meaning |
|---|---|
| Positive integer | Bytes successfully read |
| `0` | End-of-file |
| `-1` | Error |

The compile-time macro `BUFFER_SIZE` specifies how many bytes are requested per `read()` call. **It is not the maximum length of a line.**

### Buffering Across Calls

For a file containing `hello\nworld`, with `BUFFER_SIZE=4`:

```text
Read 1: "hell"
Read 2: "o\nwo"   → return "hello\n"
Unread: "wo"
Read 3: "rld"     → return "world" at EOF
```

The characters after `\n` must remain available to the next call. A final line without a newline is returned as well.

### Persistent State and Memory Ownership

A regular local buffer does not survive a function return. **Static storage** makes it possible to retain unread bytes between calls. The mandatory implementation uses a descriptor-indexed array; the bonus implementation associates its buffer with a descriptor in a linked list.

Each returned line belongs to the caller, who must release it using `free()`. The implementation manages its own internal buffers independently.

### Performance Considerations

In the original mandatory implementation, each `ft_strjoin()` copies the accumulated string into a new allocation. Repeated concatenation can lead to **quadratic copying work** for long lines, particularly when `BUFFER_SIZE=1`.

The bonus implementation uses a growing buffer and offsets to reduce repeated copying. The original mandatory source files have been preserved unchanged.

### References

- [`read(2)` — Linux manual](https://man7.org/linux/man-pages/man2/read.2.html)
- [`open(2)` — Linux manual](https://man7.org/linux/man-pages/man2/open.2.html)
- [`close(2)` — Linux manual](https://man7.org/linux/man-pages/man2/close.2.html)
- [C memory management — cppreference](https://en.cppreference.com/w/c/memory)
