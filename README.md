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

### File Descriptors and System Calls

A **file descriptor (fd)** is an integer that identifies an open input/output resource. Standard input is `0`, standard output is `1`, and standard error is `2`.

| Function | Role |
|---|---|
| `open()` | Opens a file and returns its descriptor (or `-1` on error). |
| `read()` | Reads up to a requested number of bytes; returns the byte count, `0` at EOF, or `-1` on error. |
| `close()` | Closes a file descriptor. |

The caller handles `open()` and `close()`; `get_next_line()` calls `read()` internally. Unlike a line-reading function, `read()` works with **bytes**: one call may return less than requested or include bytes from more than one line.

### How `get_next_line()` Works

`get_next_line(int fd)` returns the next line, including `\n` when present. It keeps unread bytes for the next call.

```text
get_next_line(fd)
       |
       v
Check saved buffer for '\n'
       |
       +-- Not found --> read() and append data
       |                         |
       |                     Check again
       v
Extract one line
       |
       +--> Return line (caller frees it)
       |
       +--> Save remaining bytes for next call
```

At EOF, any final line without a newline is returned before subsequent calls return `NULL`. Errors also result in `NULL`.

**Example** — input `hello\nworld` with `BUFFER_SIZE=4`:

```text
read 1: "hell"
read 2: "o\nwo"  -> return "hello\n", keep "wo"
read 3: "rld"    -> EOF -> return "world"
next call        -> NULL
```

`BUFFER_SIZE` controls how many bytes are requested in each `read()`, **not** the maximum line length.

### Functions Used

| Function | Purpose |
|---|---|
| [`get_next_line()`](get_next_line.c) | Main function: returns one line per call |
| `read_line()` / `read_chunk()` | Find a newline, read chunks, and append data |
| `join_line()` | Separate the returned line from leftover bytes |
| `free_null()` | Free an internal buffer and set its pointer to `NULL` |
| [`ft_strchr()`](get_next_line_utils.c) | Find a character such as `\n` |
| `ft_strjoin()` / `ft_substr()` | Concatenate data and extract substrings |
| `ft_strdup()` / `ft_strlen()` | Duplicate a string and calculate its length |

The mandatory implementation is in [`get_next_line.c`](get_next_line.c) and [`get_next_line_utils.c`](get_next_line_utils.c).

### Static Buffers and the Bonus

**Why `static`?** A normal local variable is lost when a function returns. A static buffer preserves unread characters between calls.

- **Mandatory:** a static array stores a separate buffer per file descriptor, with a default maximum fd of `256`.
- **Bonus:** a linked list stores state for each fd; its buffers grow as needed and track `start`, `end`, and `capacity`.

See [`get_next_line_bonus.c`](get_next_line_bonus.c) and [`get_next_line_utils_bonus.c`](get_next_line_utils_bonus.c).

### Memory Management

Each line returned by `get_next_line()` is allocated dynamically and **must be freed by the caller**, as shown in the Usage Example. The function manages its internal saved data separately.

The mandatory version may repeatedly copy its accumulated buffer via `ft_strjoin()`. The bonus version uses a growable buffer to reduce unnecessary copying.

### References

- [`read(2)` — Linux manual](https://man7.org/linux/man-pages/man2/read.2.html)
- [`open(2)` — Linux manual](https://man7.org/linux/man-pages/man2/open.2.html)
- [`close(2)` — Linux manual](https://man7.org/linux/man-pages/man2/close.2.html)
- [C memory management — cppreference](https://en.cppreference.com/w/c/memory)
