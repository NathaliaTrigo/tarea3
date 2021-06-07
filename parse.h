#pragma once

#include "common.h"

bool parse_piece(char **line_ptr, char *buffer, size_t buffer_size,
                 char separator, bool newline_is_separator);

bool string_to_int(char *string, int *result);

char *trim_whitespace(char *string);