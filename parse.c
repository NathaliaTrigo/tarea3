#include "parse.h"

bool parse_piece(char **line_ptr, char *buffer, size_t buffer_size,
                 char separator, bool newline_is_separator)
{
    char *line_at = *line_ptr;
    size_t copied = 0;
    bool found_separator = false;

    while (*line_at && copied < buffer_size)
    {
        // Iteramos line_at hasta encontrar separator.
        // O un newline si el flag es true.
        if ((*line_at == '\n' && newline_is_separator) ||
            (*line_at == separator))
        {
            found_separator = true;
            ++line_at;
            break;
        }

        if (line_at[1] == '\0')
        {
            found_separator = true;
            *buffer++ = *line_at++;
            ++copied;
            break;
        }

        // Copiamos el char actual y avanzamos
        *buffer++ = *line_at++;
        ++copied;
    }

    // Done.
    *line_ptr = line_at;
    *buffer++ = 0;
    return (found_separator);
}

bool string_to_int(char *string, int *result)
{
    bool is_valid = true;
    char *end_ptr;
    int value = strtol(string, &end_ptr, 10);
    if (string == end_ptr)
    {
        is_valid = false;
    }
    else
    {
        if (result)
        {
            *result = value;
        }
    }
    return (is_valid);
}

char *trim_whitespace(char *string)
{
    char *end;

    // Trim leading space
    while (isspace((unsigned char)*string))
        string++;

    if (*string == 0) // All spaces?
        return string;

    // Trim trailing space
    end = string + strlen(string) - 1;
    while (end > string && isspace((unsigned char)*end))
        end--;

    // Write new null terminator character
    end[1] = '\0';
    return string;
}
