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

		if ((data->file->keys = malloc(data->file->sec_len * sizeof(configr_key *))) == NULL)
		{
			return NULL;
		}

		if ((data->file->filename = calloc(1, sizeof(char))) == NULL)
		{
			return NULL;
		}
	}

	data->last_sec = 0;
	data->last_key = 0;

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

	return 0;
}

configr_key *configr_next_key(configr *configr)
{
	printf("last_sec: %i\n", configr->last_sec);
	printf("last_key: %i\n", configr->last_key);

	if (configr->last_sec == configr->file->sec_len - 1 && configr->last_key == *(configr->file->key_len + configr->last_sec) - 1)
	{
		printf("last!\n");
		return NULL;
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
