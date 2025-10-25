#pragma once

#include "core.h"
#include "list/types.h"
#include "str/types.h"

Ilist *build_next(const char *t);
Ilist *build_nextval(const char *t);
// Return first occurrence of t in s, return -1 if not found.
isize kmp(const char *s, const char *t);

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
char str_pop(Str *s);
void str_reverse(Str *s);
void str_clear(Str *s);

// Return whether c is in the string s.
bool is_char_in(char c, const char *s);

typedef struct {
	const char *seps;
} SplitArg;

// Split the string by white spaces into a list. If optional .seps given, split
// by characters in them.
#define str_split(s, ...) _str_split(s, (SplitArg){__VA_ARGS__})
// TODO: make this apply to char *
Slist *_str_split(const Str *s, SplitArg arg);
char *str_join(const Slist *list, const char *sep);

// Remove white spaces on the left.
void str_lstrip(Str *s);
// Remove white spaces on the right.
void str_rstrip(Str *s);
// Remove white spaces on the both sides.
void str_strip(Str *s);

/// Read a line from the file to the string.
/// NOTE: this overrides the string, and newline is preserved.
void str_readline(Str *s, FILE *f);
/// Read the whole file from the file descriptor to the string.
/// NOTE: this overrides the string.
void str_readfd(Str *s, int fd);
/// Read the whole file to the string.
/// NOTE: this overrides the string.
void str_readfile(Str *s, FILE *f);
