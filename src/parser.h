#ifndef __CONFIGR_PARSER_H
#define __CONFIGR_PARSER_H

#include <ctype.h>

#include "configr.h"

extern configr_key *parser_next_key(configr *configr);
extern void parser_tokenize(configr *configr, char *line);
int parser_line(configr *configr, char *line);
extern char *parser_section(char *line);
extern char *parser_trim(char *token);

#endif /* __CONFIGR_PARSER_H */
