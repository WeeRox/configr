#include "parser.h"
#if 0

configr_key *parser_next_key(configr *configr)
{
	char *line;

	while ((line = strtok_r(configr->rest_data, "\n", &configr->rest_data)))
	{
		int result = parser_line(configr, parser_trim(line));

		if (result == 1)
		{
			continue;
		}
	}

	return NULL;
}

int parser_line(configr *configr, char *line)
{
	/* check if line is empty or is a comment */
	if (line[0] == '\n' || strchr(configr->comment, line[0]))
	{
		return 1;
	}

	if (line[0] == '[') {
		configr->key->section = parser_section(line);
		configr->key->name = NULL;
		configr->key->value = NULL;
	}

	parser_tokenize(configr, line);
	if (configr->key->name == NULL)
	{
		return 1;
	}
	else
	{
		return configr->key;
	}
}

char *parser_section(char *line)
{
	char *section = calloc(1, sizeof(char));

	for (size_t i = 1; i < strlen(line); i++)
	{
		if (line[i] != ']')
		{
			int len = strlen(section);
			section = realloc(section, len + 1 + 1); /* one char and one NUL char */
			section[len] = line[i];
			section[len + 1] = '\0';
		}
		else
		{
			return section;
		}
	}
	/* invalid section */
	return NULL;
}

void parser_tokenize(configr *configr, char *line)
{
	/* parse name and value */
	int assign = 0;
	char *name = calloc(1, sizeof(char));
	char *value = calloc(1, sizeof(char));

	for (size_t i = 0; i < strlen(line); i++)
	{
		if (strchr(configr->assign, line[i]))
		{
			assign = -1;
		}
		else if (strchr(configr->comment, line[i]))
		{
			/* this char is a comment char */
			break;
		}
		else
		{
			if (!assign)
			{
				int len = strlen(name);
				name = realloc(name, len + 1 + 1); /* one char and one NUL char */
				name[len] = line[i];
				name[len + 1] = '\0';
			}
			else
			{
				int len = strlen(value);
				value = realloc(value, len + 1 + 1); /* one char and one NUL char */
				value[len] = line[i];
				value[len + 1] = '\0';
			}
		}
	}

	if (strlen(name) == 0 || strlen(value) == 0)
	{
		configr->key->name = NULL;
		configr->key->value = NULL;
		/* TODO: return -1? */
	}
	else
	{
		name = parser_trim(name);
		configr->key->name = realloc(configr->key->name, (strlen(name) + 1) * sizeof(char));
		strcpy(configr->key->name, name);

		value = parser_trim(value);
		configr->key->value = realloc(configr->key->value, (strlen(value) + 1) * sizeof(char));
		strcpy(configr->key->value, value);
	}

	free(name);
	free(value);
}

char *parser_trim(char *token)
{
	char *trim = calloc(1, (strlen(token) + 1) * sizeof(char));
	int len = 0;
	int last_space = -1;

	for (size_t i = 0; i < strlen(token); i++)
	{
		if (!isspace(token[i]))
		{
			last_space = -1;
		}
		else if (isspace(token[i]) && len != 0)
		{
			if (last_space == -1)
			{
				last_space = len;
			}
		}
		else
		{
			continue;
		}

		trim[len] = token[i];
		len++;
	}

	if (last_space != -1)
	{
		trim[last_space] = '\0';
	}

	return trim;
}
#endif
