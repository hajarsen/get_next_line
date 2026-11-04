# get_next_line

*C · 42 curriculum*

`get_next_line(int fd)` reads from a file descriptor one line at a time. It returns the next line, including the terminating newline when present, or `NULL` at end-of-file or on error. The caller must free each returned line.

## Implementation

The mandatory version preserves unread input between calls. The bonus version maintains separate read state for each file descriptor, with a dynamically growing buffer.

## Compilation

Compile the mandatory (`get_next_line.c`, `get_next_line_utils.c`) or bonus (`get_next_line_bonus.c`, `get_next_line_utils_bonus.c`) implementation separately. `BUFFER_SIZE` can be configured at compile time with `-D BUFFER_SIZE=<n>`.
