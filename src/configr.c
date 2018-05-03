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

	if ((data->key = malloc(sizeof(configr_key))) == NULL)
	{
		return NULL;
	}
	else
	{
		if ((data->key->section = (char *) calloc(1, sizeof(char))) == NULL)
		{
			return NULL;
		}

		if ((data->key->name = (char *) calloc(1, sizeof(char))) == NULL)
		{
			return NULL;
		}

		if ((data->key->value = (char *) calloc(1, sizeof(char))) == NULL)
		{
			return NULL;
		}
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
	return parser_next_key(configr);
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

	if (configr->key)
	{
		if (configr->key->section)
		{
			free(configr->key->section);
		}

		if (configr->key->name)
		{
			free(configr->key->name);
		}

		if (configr->key->value)
		{
			free(configr->key->value);
		}

		free(configr->key);
	}

	if (configr->comment)
	{
		free(configr->comment);
	}

	if (configr->assign)
	{
		free(configr->assign);
	}

	free(configr);
}
