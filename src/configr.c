#include "configr.h"
#include "parser.h"

configr *configr_init()
{
	configr *data = malloc(sizeof(configr));

	if ((data->data = calloc(1, sizeof(char))) == NULL)
	{
		return NULL;
	}

	if ((data->rest_data = calloc(1, sizeof(char))) == NULL)
	{
		return NULL;
	}

	if ((data->comment = malloc((strlen(CONFIGROPT_COMMENT_DEFAULT) + 1) * sizeof(char))) == NULL)
	{
		return NULL;
	}
	else
	{
		strcpy(data->comment, CONFIGROPT_COMMENT_DEFAULT);
	}

	if ((data->assign = malloc((strlen(CONFIGROPT_ASSIGN_DEFAULT) + 1) * sizeof(char))) == NULL)
	{
		return NULL;
	}
	else
	{
		strcpy(data->assign, CONFIGROPT_ASSIGN_DEFAULT);
	}

	if ((data->file = (configr_file *) malloc(sizeof(configr_file))) == NULL)
	{
		return NULL;
	}
	else
	{
		data->file->sec_len = 1; /* 1 for global config keys */

		if ((data->file->key_len = malloc(data->file->sec_len * sizeof(int))) == NULL)
		{
			return NULL;
		}
		else
		{
			*(data->file->key_len) = 0; /* no global keys on init */
		}

		if ((data->file->sections = malloc(data->file->sec_len * sizeof(char *))) == NULL)
		{
			return NULL;
		}
		else
		{
			if ((*(data->file->sections) = calloc(1, sizeof(char))) == NULL)
			{
				return NULL;
			}
		}

		if ((data->file->keys = malloc(data->file->sec_len * sizeof(configr_key **))) == NULL)
		{
			return NULL;
		}
		else
		{
			if ((*data->file->keys = malloc(*data->file->key_len * sizeof(configr_key *))) == NULL)
			{
				return NULL;
			}
		}

		if ((data->file->filename = calloc(1, sizeof(char))) == NULL)
		{
			return NULL;
		}
	}

	data->curr_sec = 0;
	data->curr_key = 0;

	return data;
}

int configr_setopt(configr *configr, configr_option option, void *value)
{
	if (option == CONFIGROPT_COMMENT)
	{
		if ((configr->comment = realloc(configr->comment, (strlen(value) + 1) * sizeof(char))) == NULL)
		{
			return -1;
		}

		strcpy(configr->comment, value);
	}
	else if (option == CONFIGROPT_ASSIGN)
	{
		if ((configr->assign = realloc(configr->assign, (strlen(value) + 1) * sizeof(char))) == NULL)
		{
			return -1;
		}

		strcpy(configr->assign, value);
	}
	else
	{
		/* unknown option */
		return -1;
	}

	return 0;
}

int configr_parse_stream(configr *configr, FILE *stream)
{
	#define BUFFER_SIZE 1024

	char buffer[BUFFER_SIZE];
	size_t size = 1; /* just a NULL char */
	char *content = malloc(sizeof(char) * BUFFER_SIZE);

	if (content == NULL)
	{
		return -1;
	}

	content[0] = '\0';

	while (fgets(buffer, BUFFER_SIZE, stream))
	{
		char *old = content;
		size += strlen(buffer);
		content = realloc(content, size);

		if (content == NULL)
		{
			free(old);
			return -1;
		}

		strcat(content, buffer);
	}

	configr->data = realloc(configr->data, (strlen(content) + 1) * sizeof(char));
	strcpy(configr->data, content);
	configr->rest_data = configr->data;

	free(content);

	parse_data(configr);

	return 0;
}

int configr_parse_file(configr *configr, char *filename)
{
	FILE *stream = fopen(filename, "r");
	return configr_parse_stream(configr, stream);
}

int configr_parse_string(configr *configr, char *str)
{
	configr->data = realloc(configr->data, (strlen(str) + 1) * sizeof(char));
	strcpy(configr->data, str);
	configr->rest_data = configr->data;

	parse_data(configr);

	return 0;
}

configr_key *configr_next_key(configr *configr)
{

	if (configr->file->sec_len == 1 && *configr->file->key_len == 0)
	{
		/* no keys exist */
		configr->curr_key = 0;
		configr->curr_sec = 0;
		return NULL;
	}

	if (configr->curr_sec == configr->file->sec_len)
	{
			/* we have passed the last section */
			/* reset variables to be able to loop keys again */
			configr->curr_sec = 0;
			configr->curr_key = 0;

			return NULL;
	}

	configr_key *result;

	if (configr->curr_key == *(configr->file->key_len + configr->curr_sec))
	{
		configr->curr_sec++;
		configr->curr_key = 0;
		result = configr_next_key(configr);
	}
	else
	{
		result = *(*(configr->file->keys + configr->curr_sec) + configr->curr_key);
		configr->curr_key++;
	}

	return result;
}

int configr_add_key(configr *configr, char *section, char *name, char *value)
{
	int pos = configr->file->sec_len;
	for (size_t i = 0; i < configr->file->sec_len; i++) {
		if (strcmp(*(configr->file->sections + i), section) == 0)
		{
			pos = i;
		}
	}

	if (pos == configr->file->sec_len)
	{
		/* create a new section */

		/* add the new section name */
		configr->file->sections = realloc(configr->file->sections, (configr->file->sec_len + 1) * sizeof(char *));
		if (configr->file->sections == NULL)
		{
			fprintf(stderr, "Error reallocating memory\n");
			exit(1);
		}

		*(configr->file->sections + configr->file->sec_len) = malloc((strlen(section) + 1) * sizeof(char));
		strcpy(*(configr->file->sections + configr->file->sec_len), section);

		/* add new section to key array */
		configr->file->keys = realloc(configr->file->keys, (configr->file->sec_len + 1) * sizeof(configr_key **));
		if (configr->file->keys == NULL)
		{
			fprintf(stderr, "Error reallocating memory\n");
			exit(1);
		}

		/* add section to key length */
		configr->file->key_len = realloc(configr->file->key_len, (configr->file->sec_len + 1) * sizeof(int));
		*(configr->file->key_len + configr->file->sec_len) = 0;

		/* malloc size of new section (0) */
		*(configr->file->keys + configr->file->sec_len) = malloc(*(configr->file->key_len + configr->file->sec_len) * sizeof(configr_key *));

		configr->file->sec_len += 1;
	}

	configr_key *new = malloc(sizeof(configr_key));
	new->section = *(configr->file->sections + pos);

	new->name = malloc((strlen(name) + 1) * sizeof(char));
	strcpy(new->name, name);

	new->value = malloc((strlen(value) + 1) * sizeof(char));
	strcpy(new->value, value);

	*(configr->file->keys + pos) = realloc(*(configr->file->keys + pos), (*(configr->file->key_len + pos) + 1) * sizeof(configr_key *));

	if (*(configr->file->keys + pos) == NULL)
	{
		fprintf(stderr, "Error reallocating memory\n");
		exit(1);
	}

	*(*(configr->file->keys + pos) + *(configr->file->key_len + pos)) = new;
	*(configr->file->key_len + pos) += 1;

	return 0;
}

configr_key *configr_get_key(configr *configr, char *section, char *name)
{
	int pos = -1;
	for (size_t i = 0; i < configr->file->sec_len; i++) {
		if (strcmp(*(configr->file->sections + i), section) == 0)
		{
			pos = i;
		}
	}

	if (pos != -1)
	{
		for (size_t i = 0; i < *(configr->file->key_len + pos); i++) {
			configr_key *key = *(*(configr->file->keys + pos) + i);
			if (strcmp(key->name, name) == 0)
			{
				return key;
			}
		}
	}

	return NULL;
}

void configr_free(configr *configr)
{
	if (configr->data)
	{
		free(configr->data);
	}

	if (configr->rest_data)
	{
		free(configr->rest_data);
	}

	if (configr->comment)
	{
		free(configr->comment);
	}

	if (configr->assign)
	{
		free(configr->assign);
	}

	if (configr->file)
	{
		if (configr->file->key_len)
		{
			free(configr->file->key_len);
		}

		if (configr->file->sections)
		{
			free(configr->file->sections);
		}

		if (configr->file->keys)
		{
			free(configr->file->keys);
		}

		if (configr->file->filename)
		{
			free(configr->file->filename);
		}

		free(configr->file);
	}

	free(configr);
}
