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

### 1. File Descriptors: Where the Bytes Come From

A **file descriptor (fd)** is a small integer that a process uses to refer to an open input/output resource. The operating system manages the underlying resource; the integer is the handle your C program passes to system calls.

| File descriptor | Usual meaning |
|---|---|
| `0` | Standard input (`stdin`) |
| `1` | Standard output (`stdout`) |
| `2` | Standard error (`stderr`) |
| `3`, `4`, ... | Other open files or input/output resources (numbers depend on the process) |

```text
                     One process
             +------------------------+
             | File descriptor table  |
keyboard --->| fd 0 : standard input |
terminal <---| fd 1 : standard output|
terminal <---| fd 2 : standard error |
             | fd 3 : open file -----+----> file on disk
             +------------------------+

           fd = an integer, NOT a file path
```

**Important:** `get_next_line(fd)` does **not** open a file. It reads from an fd that the **caller already has**, such as an open text file or standard input.

### 2. `open()`, `read()`, and `close()`

These are low-level POSIX system interfaces declared through `<fcntl.h>` (for `open`) and `<unistd.h>` (for `read` and `close`).

| Function | Purpose | Typical result |
|---|---|---|
| `open(path, flags)` | Open a file and obtain an fd | Nonnegative fd on success, `-1` on error |
| `read(fd, buf, count)` | Request **up to** `count` bytes | Positive byte count, `0` for EOF, `-1` on error |
| `close(fd)` | Release that process's descriptor | `0` on success, `-1` on error |

The normal sequence when reading a regular file is:

```text
      "example.txt"
            |
            v
   open("example.txt", O_RDONLY)
            |
            v
       fd (e.g. 3)
            |
            v
      get_next_line(fd)
            |
      calls read(fd, ...)
            |
       returns one line
            |
            v
     caller uses + frees
      the returned line
            |
       repeat until NULL
            |
            v
         close(fd)
```

The caller is responsible for `open()` and `close()`; your `get_next_line()` implementation handles the internal `read()` calls.

**What `read()` actually returns:**

```c
ssize_t n = read(fd, buffer, BUFFER_SIZE);

if (n > 0)  /* n bytes received; may be fewer than BUFFER_SIZE */
if (n == 0) /* end-of-file: no more bytes */
if (n < 0)  /* read error */
```

- `read()` reads **bytes, not lines**. A single read may contain half a line, several lines, or just one byte.
- A successful read is **not guaranteed to fill** the requested buffer.
- On regular files, successive reads normally advance the current file position.
- `BUFFER_SIZE` is the requested chunk size, **not a maximum line length**.

Your implementation uses `read()` in [`get_next_line.c`](get_next_line.c) and [`get_next_line_bonus.c`](get_next_line_bonus.c).

### 3. How `get_next_line()` Builds a Line

The function's responsibility is to return **one line per call**, including the newline (`\n`) when it exists. It must save any bytes *after* that newline for the next call.

```text
          get_next_line(fd)
                 |
                 v
       Saved bytes contain '\n'?
          /                \
        yes                 no
         |                   |
         |           read(fd, chunk, BUFFER_SIZE)
         |                   |
         |           append bytes to saved buffer
         |                   |
         |              newline found?
         |                /      \
         |              no        yes
         |              |          |
         |          read again     |
         +-------------------------+
                 |
                 v
       Split at first newline
            /          \
           v            v
    Return one line   Keep leftover
    (caller frees)    for next call

    EOF + leftover -> return final line without '\n'
    EOF + no data  -> return NULL
    read error     -> return NULL
```

The public API is:

```c
char *get_next_line(int fd);
```

The returned string is dynamically allocated. The caller must `free()` it after use; `NULL` signals end-of-input or an error (the interface does not distinguish them).

### 4. Buffering Across Calls: Worked Example

For a file containing `hello\nworld`, with `BUFFER_SIZE=4`:

```text
Underlying byte stream:
h e l l o \n w o r l d
|_____| |_____| |___|
 read 1  read 2  read 3

1) read -> "hell"     saved buffer: "hell"
   No '\n' yet.

2) read -> "o\nwo"    saved buffer: "hello\nwo"
                              |
                              +-- split here

   get_next_line() returns "hello\n"
   leftover saved for next call: "wo"

3) Next get_next_line() call:
   read -> "rld"      saved buffer: "world"
   read -> 0 (EOF)
   returns "world" (no trailing newline)

4) Next call returns NULL.
```

**Why not discard the remainder?** Because `read()` may receive bytes belonging to the *next* line. Those bytes have already been consumed from the file descriptor and cannot simply be read again.

### 5. Key Functions in This Repository

The **mandatory** version uses these functions to turn a byte stream into lines:

| Function | Role in the implementation |
|---|---|
| [`get_next_line()`](get_next_line.c) | Entry point; retrieves the next line for one fd |
| `read_line()` | Searches for a newline, reads more chunks if necessary |
| `read_chunk()` | Calls `read()` and appends new bytes to the saved buffer |
| `join_line()` | Splits a completed line from the unread remainder |
| `free_null()` | Frees an internal buffer and clears its pointer |
| [`ft_strchr()`](get_next_line_utils.c) | Finds a newline character |
| `ft_strjoin()` | Concatenates saved bytes and the latest chunk |
| `ft_substr()` | Extracts a line or the remaining bytes |
| `ft_strdup()` | Creates an owned copy of a string |
| `ft_strlen()` | Computes string length |

`read_line()`, `read_chunk()`, `join_line()`, and `free_null()` are `static` helpers defined in [`get_next_line.c`](get_next_line.c). The string utilities live in [`get_next_line_utils.c`](get_next_line_utils.c).

### 6. Persistent State: Mandatory vs Bonus

A local variable disappears when its function returns. However, `get_next_line()` must remember unread bytes between calls. Both versions therefore use **static state**, but organize it differently.

**Mandatory:** an array of buffer pointers, indexed by fd:

```text
static char *buffer[MAX_FD + 1];

buffer[0]  ---> pending bytes from stdin
buffer[3]  ---> pending bytes from file A
buffer[4]  ---> pending bytes from file B
   ...
buffer[256] ---> last allowed index (default MAX_FD)
```

This design allows separate saved strings for different descriptors, but rejects any fd larger than `MAX_FD` (256 by default).

**Bonus:** a linked list of descriptor-specific state, with a growable buffer:

```text
static head
    |
    v
[fd=3 | data,start,end,capacity] --next-->
[fd=5 | data,start,end,capacity] --next--> NULL
```

- `gnl_node()` finds or creates a descriptor entry.
- `gnl_fill()` calls `read()` and appends incoming bytes.
- `gnl_take_line()` copies one line and advances the start offset.
- `gnl_remove()` cleans up an entry on EOF or error.
- `gnl_append()` / `gnl_grow()` expand storage when needed.
- `gnl_newline()` locates the newline within the current byte range.

See [`get_next_line_bonus.c`](get_next_line_bonus.c) and [`get_next_line_utils_bonus.c`](get_next_line_utils_bonus.c).

### 7. Memory and Performance

A `get_next_line()` result belongs to the **caller**:

```c
char *line = get_next_line(fd);
if (line)
{
    /* use line */
    free(line);
}
```

The internal saved buffer is separate from the returned line, so freeing one returned line does not discard the bytes needed by the next call.

In the mandatory version, repeatedly calling `ft_strjoin()` copies all accumulated text into a new allocation. For a very long line (especially with `BUFFER_SIZE=1`), this can cause **quadratic copying work** in the line length.

The bonus version uses a buffer with `start`, `end`, and `capacity` offsets and expands its capacity when needed. This reduces repeated copying of accumulated bytes.

**Edge cases worth checking:** empty input, missing final newline, multiple consecutive newlines, very long lines, `BUFFER_SIZE=1`, invalid descriptors, and interleaved reads from two distinct fds.

### References

- [`read(2)` — Linux manual](https://man7.org/linux/man-pages/man2/read.2.html)
- [`open(2)` — Linux manual](https://man7.org/linux/man-pages/man2/open.2.html)
- [`close(2)` — Linux manual](https://man7.org/linux/man-pages/man2/close.2.html)
- [C memory management — cppreference](https://en.cppreference.com/w/c/memory)
