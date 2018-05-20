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
	int sec_len;					/* number of sections */
	int *key_len;					/* number of keys in section n */
	char **sections; 			/* name of section n */
	configr_key ***keys;	/* array of keys */
	char *filename;				/* filename */
} configr_file;

typedef struct configr
{
	char *data;				/* data to be parsed */
	char *rest_data;	/* pointer to unparsed data */

	char *comment;		/* comment chars */
	char *assign;			/* assignment chars */

	configr_file *file;

	int curr_sec;			/* current section used in configr_next_key */
	int curr_key;			/* current key used in configr_next_key */
} configr;

#define CONFIGROPT_COMMENT_DEFAULT "#;"	/* default comment chars */
#define CONFIGROPT_ASSIGN_DEFAULT "=:"	/* default assignment chars */

typedef enum {
	/* chars used to identify beginning of comment */
	CONFIGROPT_COMMENT,

	/* chars used to identify assignment */
	CONFIGROPT_ASSIGN
} configr_option;


/*
 * NAME
 * configr_init
 *
 * DESCRIPTION
 * Create a configr handle
 *
 * RETURNS
 * An initiated configr handle
*/
extern configr *configr_init(void);

/*
 * NAME
 * configr_setopt
 *
 * DESCRIPTION
 * Set option for a configr handle
 *
 * PARAMETERS
 * configr	Initiated handle
 * option		Enum defining what option to set
 * value		Value to assign to option
 *
 * RETURNS
 * 0 on success, -1 on failure
*/
extern int configr_setopt(configr *configr, configr_option option, void *value);

/*
 * NAME
 * configr_free
 *
 * DESCRIPTION
 * Destroy configr handle and release the memory associated with it
 *
 * PARAMETERS
 * configr	Handle to be destroyed
*/
extern void configr_free(configr *configr);

/*
 * NAME
 * configr_parse_stream
 *
 * DESCRIPTION
 * Parse configuration from stream
 *
 * PARAMETERS
 * configr	Handle to save parsed data to
 * stream		Stream to read data from
 *
 * RETURNS
 * 0 on success, -1 on failure
*/
extern int configr_parse_stream(configr *configr, FILE *stream);

/*
 * NAME
 * configr_parse_file
 *
 * DESCRIPTION
 * Parse configuration from file
 *
 * PARAMETERS
 * configr	Handle to save parsed data to
 * filename	Path and name of file
 *
 * RETURNS
 * 0 on success, -1 on failure
*/
extern int configr_parse_file(configr *configr, char *filename);

/*
 * NAME
 * configr_parse_string
 *
 * DESCRIPTION
 * Parse configuration from string
 *
 * PARAMETERS
 * configr	Handle to save parsed data to
 * str			String from which to read data
 *
 * RETURNS
 * 0 on success, -1 on failure
*/
extern int configr_parse_string(configr *configr, char *str);

/*
 * NAME
 * configr_next_key
 *
 * DESCRIPTION
 * Get next key in file, used to loop through all keys
 * When the last key had been reached the counter will reset back to the start
 *
 * PARAMETERS
 * configr	Handle containing keys
 *
 * RETURNS
 * conigr_key object or NULL if there are no keys left
*/
extern configr_key *configr_next_key(configr *configr);

/*
 * NAME
 * configr_add_key
 *
 * DESCRIPTION
 * Add key to configuration file
 *
 * PARAMETERS
 * configr	Handle containing file to add key to
 * section	Section to which to add the key to,
 * 					empty string to add as global key
 * name			Name of key
 * value		Value of key
 *
 * RETURNS
 * 0 on success, -1 on failure
*/
extern int configr_add_key(configr *configr, char *section, char *name, char *value);

/*
 * NAME
 * configr_get_key
 *
 * DESCRIPTION
 * Retrieve key from file if it exists
 *
 * PARAMETERS
 * configr	Handle containing file with key which to retreive
 * section	Section in which key is located
 * name			Name of key
 *
 * RETURNS
 * configr_key object or NULL if key doesn't exist
*/
extern configr_key *configr_get_key(configr *configr, char *section, char *name);

#endif /* __CONFIGR_CONFIGR_H */
