#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cencode.h"

void b64enc(char **enc, char *str)
{
	size_t length = strlen(str);
	char *encstring = malloc(length+1);
	base64_encodestate state;
	base64_init_encodestate(&state);
	length = base64_encode_block(str, length, encstring, &state);
	base64_encode_blockend(encstring + length, &state);
	//if (encstring[strlen(encstring) -1] == '\n') {encstring[strlen(encstring) - 1] = '\0';}
	char *tmp = (char *)malloc(strlen(encstring));
	strcpy(tmp, encstring);
	*enc = tmp;
}
