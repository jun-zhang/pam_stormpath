#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <syslog.h>

#include <security/pam_appl.h>
#include <security/pam_modules.h>
#include <security/pam_ext.h>

#include "req.h"
#include "config.h"

int do_sp_auth (char* u, char* p)
{
	int rc = 0;

	rc = make_req(u, p);
	return rc;
}

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

PAM_EXTERN int pam_sm_setcred( pam_handle_t *pamh, int flags, int argc, const char **argv ) {
	return PAM_SUCCESS;
}

PAM_EXTERN int pam_sm_acct_mgmt(pam_handle_t *pamh, int flags, int argc, const char **argv) {
	return PAM_SUCCESS;
}

PAM_EXTERN int pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
	int retval, rc;
	const char *username, *password;
	openlog("pam_stormpath", 0, LOG_AUTHPRIV);

	retval = pam_get_user(pamh, &username, "login: ");
	if (retval == PAM_SUCCESS)
	{
		syslog(LOG_INFO, "Login attempt for user: %s", username);
	} else {
		syslog(LOG_INFO, "unable to get username");
		goto done;
	}

	retval = pam_get_authtok(pamh, PAM_AUTHTOK, (const char **) &password, NULL);
	if (retval == PAM_SUCCESS)
	{
		if (password == NULL)
		{
			syslog(LOG_INFO, "No password detected");
			goto done;
	      	}

		syslog(LOG_INFO, "Authing user against stormpath");
		rc = do_sp_auth((char *) username, (char *) password);
		syslog(LOG_INFO, "rc: %d", rc);
		if (rc == 0) {
			retval = PAM_SUCCESS;
			syslog(LOG_INFO, "Successful auth");
			goto done;
		} else {
			retval = PAM_AUTH_ERR;
			syslog(LOG_INFO, "Failed auth");
			goto done;
		}
	}

done:
	pam_set_data(pamh, "stormpath_setcred_return", (void *) (intptr_t) retval, NULL);
	return retval;
}
