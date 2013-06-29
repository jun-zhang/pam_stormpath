#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>

int sp_config(char **id, char **secret, char **app) {
	openlog("pam_stormpath", 0, LOG_AUTHPRIV);

	FILE *fp;
	char* line = NULL;
	char* token;
        char* saveptr;
	int rc = 0;

	size_t len = 0;
	ssize_t read;


	if (!(fp = fopen("/etc/pam_stormpath.conf", "r")))
        {
                syslog(LOG_ALERT,"failed to read configuration file");
                return 1;
        }

	while ((read = getline(&line, &len, fp)) != -1)
	{
                token = strtok_r(line, "=", &saveptr);
                if (token) {
                        if (strcmp(token, "SPID") == 0) {
                                token = strtok_r(NULL, "=", &saveptr);
                                if (token) {
                                        if (token[strlen(token) - 1] == '\n') {
                                                token[strlen(token) - 1] = '\0';
                                        }
                                        *id = strdup(token);
                                }
                        }
                        else if (strcmp(token, "SPSECRET") == 0) {
                                token = strtok_r(NULL, "=", &saveptr);
                                if (token) {
                                        if (token[strlen(token) - 1] == '\n') {
                                                token[strlen(token) - 1] = '\0';
                                        }
                                        *secret = strdup(token);
                                }
                        }
                        else if (strcmp(token, "SPAPPLICATION") == 0) {
                                token = strtok_r(NULL, "=", &saveptr);
                                if (token) {
                                        if (token[strlen(token) - 1] == '\n') {
                                                token[strlen(token) - 1] = '\0';
                                        }
                                        *app = strdup(token);
                                }
                        }
                }
	}
        free(line);

	if ((*id == NULL) || (*secret == NULL) || (*app  == NULL))
	{
		rc = 1;
	} else {
		syslog(LOG_INFO, "id: %s", *id);
		syslog(LOG_INFO, "secret: %s", *secret);
		syslog(LOG_INFO, "app: %s", *app);	
		rc = 0;
	}

	return rc;
}
