#pragma once

#include "core.h"
#include "list/types.h"
#include "str/types.h"

Ilist *build_next(char *t);
Ilist *build_nextval(char *t);
isize kmp(char *s, char *t);

bool str_start_with(const char *s, const char *pattern);
bool str_end_with(const char *s, const char *pattern);
// Remove prefix in place, if pattern not found, do nothing.
void str_remove_prefix(char *s, const char *pattern);
// Remove postfix in place, if pattern not found, do nothing.
void str_remove_postfix(char *s, const char *pattern);

Str *str_new_with_size(usize size);
Str *str_new();
Str *str_from(const char *s);
void str_free(Str *s);

void str_insert_char(Str *s, const usize i, const char c);
void str_insert_str(Str *s, const usize i, const char *t);
void str_push(Str *s, const char c);
void str_push_str(Str *s, const char *t);
char str_delete(Str *s, const usize i);

/// Read a line from the file to the string.
void str_readline(Str *s, FILE *f);
