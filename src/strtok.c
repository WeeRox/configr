#ifndef HAVE_STRTOK_R

#include <string.h>

char *configr_strtok_r(char *str, const char *delim, char **saveptr)
{
	char *token;

	if (str == NULL)
	{
		str = *saveptr;
	}

	str += strspn(str, delim);
	if (*str == '\0')
	{
		*saveptr = str;
		return NULL;
	}

	token = str;
	str = strpbrk(token, delim);
	if (str == NULL)
	{
		*saveptr = strchr(token, '\0');
	}
	else
	{
		*str = '\0';
		*saveptr = str + 1;
	}

	return token;
}

#endif /* HAVE_STRTOK_R */
