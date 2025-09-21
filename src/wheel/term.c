#include <stdio.h>

#include "term.h"

void term_clear() { printf("\e[2J\e[1;1H"); }
