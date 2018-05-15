#ifndef __CONFIGR_PARSER_H
#define __CONFIGR_PARSER_H

#include <ctype.h>

#include "configr.h"

#ifndef HAVE_STRTOK_R
extern char *configr_strtok_r(char *str, const char *delim, char **saveptr);
#define strtok_r configr_strtok_r
#endif /* HAVE_STRTOK_R */

typedef struct parser_data
{
	char *section;
} parser_data;

extern int parse_data(configr *configr);
extern int parse_line(configr *configr, parser_data *parser_data, char *line);
extern char *parse_section(char *line);
extern int parser_tokenize(configr *configr, parser_data *parser_data, char *line);
extern char *parser_trim(char *token);

#endif /* __CONFIGR_PARSER_H */
