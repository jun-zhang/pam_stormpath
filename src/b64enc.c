#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cencode.h"

void b64enc(char **enc, char *str)
{
	size_t length = strlen(str);
        /*
         * BASE64 needs 4 * ceil(n / 3) bytes, including padding
         * rather than linking in the math library or trying to
         * implement a celing function, just pessimistically round
         * up by 3
         * and add 1 for the NULL terminator
         */
        size_t enc_length = ((4 * length) / 3) + 3 + 1;
	char *encstring = malloc(enc_length);
        if (!encstring)
        {
                // TODO handle error
                return;
        }
        memset(encstring, 0, enc_length);

	base64_encodestate state;
	base64_init_encodestate(&state);
	length = base64_encode_block(str, length, encstring, &state);
	base64_encode_blockend(encstring + length, &state);
	*enc = encstring;
}
