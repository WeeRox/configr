#ifndef __CONFIGR_CONFIGR_H
#define __CONFIGR_CONFIGR_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct configr_key
{
	char *section;
	char *name;
	char *value;
} configr_key;

typedef struct configr_file
{
	int sec_len;				/* number of sections */
	int *key_len;				/* number of keys in section n */
	char **sections; 		/* name of section n */
	configr_key **keys;	/* array of keys */
	char *filename;			/* filename */
} configr_file;

typedef struct configr
{
	char *data;				/* data to be parsed */
	char *rest_data;	/* pointer to unparsed data */

	char *comment;		/* comment chars */
	char *assign;			/* assignment chars */

	configr_file *file;

	int last_sec;
	int last_key;
} configr;

#define CONFIGROPT_COMMENT_DEFAULT "#;"	/* default comment chars */
#define CONFIGROPT_ASSIGN_DEFAULT "=:"	/* default assignment chars */

typedef enum {
	/* chars used to identify beginning of comment */
	CONFIGROPT_COMMENT,

	/* chars used to identify assignment */
	CONFIGROPT_ASSIGN
} configr_option;

extern configr *configr_init(void);

extern int configr_setopt(configr *configr, configr_option option, void *value);
extern void configr_free(configr *configr);

extern int configr_parse_stream(configr *configr, FILE *stream);
extern int configr_parse_file(configr *configr, char *filename);
extern int configr_parse_string(configr *configr, char *str);

extern configr_key *configr_next_key(configr *configr);

#endif /* __CONFIGR_CONFIGR_H */
