#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <syslog.h>

#include <security/pam_appl.h>
#include <security/pam_modules.h>
#include <security/pam_ext.h>

#include "req.h"
#include "spconfig.h"

int main (int argc, char* argv[])
{
	char *username, *password;
	int rc = 0;

	if (argc == 3)
	{
		username = argv[1];
		password = argv[2];
		rc = do_sp_auth(username, password);
	} else {
		rc = 2;
	}
	return rc;
}
