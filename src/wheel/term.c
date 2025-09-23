#include <stdio.h>

#include "term.h"

void term_clear() { printf("\e[2J\e[1;1H"); }

void term_clear_line() { printf("\e[F\e[2K"); }
