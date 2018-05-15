#include "parser.h"

int parse_data(configr *configr)
{
	char *line;

	parser_data *parser_data = malloc(sizeof(parser_data));
	parser_data->section = calloc(1, sizeof(char));

	while ((line = strtok_r(configr->rest_data, "\n", &configr->rest_data)))
	{
		/* TODO: multiline support */

		int result = parse_line(configr, parser_data, parser_trim(line));
	}

	return 0;
}

/*
	parses line
	return 0 on success
	return 1 on empty or commented line
	return 2 on section
	return 3 on invalid key
*/
int parse_line(configr *configr, parser_data *parser_data, char *line)
{
	/* check if line is empty or is a comment */
	if (line[0] == '\n' || strchr(configr->comment, line[0]))
	{
		return 1;
	}

	if (line[0] == '[') {
		parser_data->section = parse_section(line);
		return 2;
	}

	return parser_tokenize(configr, parser_data, line);
}

char *parse_section(char *line)
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

/*
	tokenizes line
	return 0 on success
	return 2 on empty name or value
*/
int parser_tokenize(configr *configr, parser_data *parser_data, char *line)
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

	name = parser_trim(name);
	value = parser_trim(value);

	if (strlen(name) == 0 || strlen(value) == 0)
	{
		return 1;
	}
	else
	{
		configr_add_key(configr, parser_data->section, name, value);
		return 0;
	}
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
