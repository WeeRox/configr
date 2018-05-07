#ifndef __CONFIGR_PARSER_H
#define __CONFIGR_PARSER_H

#include <ctype.h>

#include "configr.h"

#ifndef HAVE_STRTOK_R
extern char *configr_strtok_r(char *str, const char *delim, char **saveptr);
#define strtok_r configr_strtok_r
#endif /* HAVE_STRTOK_R */

extern configr_key *parser_next_key(configr *configr);
extern void parser_tokenize(configr *configr, char *line);
int parser_line(configr *configr, char *line);
extern char *parser_section(char *line);
extern char *parser_trim(char *token);

#endif /* __CONFIGR_PARSER_H */
