#pragma once

#include "core.h"
#include "list/types.h"
#include "str/types.h"

Ilist *build_next(char *t);
Ilist *build_nextval(char *t);
isize kmp(char *s, char *t);

Str *str_new();
Str *str_from(char *s);
void str_free(Str *s);

void str_insert(Str *s, usize i, char c);
void str_insert_str(Str *s, usize i, char *t);
void str_push(Str *s, char c);
void str_push_str(Str *s, char *t);
usize str_delete(Str *s, usize i);

/// Read a line from the file to the string.
void str_readline(Str *s, FILE *f);
