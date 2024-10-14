#ifndef __PARSER_H
#define __PARSER_H

#include "types.h"

command_t *parse_input(char *input);

void free_command(command_t *cmd);

#endif // __PARSER_H