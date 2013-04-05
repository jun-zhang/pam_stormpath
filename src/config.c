#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>

int sp_config(char **id, char **secret, char **app) {
	openlog("pam_stormpath", 0, LOG_AUTHPRIV);

	FILE *fp;
	char* line = NULL;
	char* line_copy = NULL;
	char* token;

	typedef struct _sp_params { char *spid; char *spsecret; char *spapplication; } _sp_params;

	_sp_params params;
	_sp_params *p_params;
	p_params = &params;
	int rc = 0;

	size_t len = 0;
	ssize_t read;
	fp = fopen("/etc/pam_stormpath.conf", "r");
	if (fp == NULL) { syslog(LOG_ALERT,"failed to read configuration file"); return 1; }
	while ((read = getline(&line, &len, fp)) != -1)
	{
		line_copy = strdup(line);
		token = strsep(&line_copy, "=");
		if (strcmp(token, "SPID") == 0)
		{
			token = strsep(&line_copy, "=");
			p_params->spid = token;
		}
		if (strcmp(token, "SPSECRET") == 0)
		{
			token = strsep(&line_copy, "=");
			p_params->spsecret = token;
		}
		if (strcmp(token, "SPAPPLICATION") == 0)
		{
			token = strsep(&line_copy, "=");
			p_params->spapplication = token;
		}
	}
	if ((p_params->spid == NULL) || (p_params->spsecret == NULL) || (p_params->spapplication  == NULL))
	{
		rc = 1;
	} else {
		char *tmpid = (char *)malloc(strlen(p_params->spid + 1));
		strcpy(tmpid, p_params->spid);
		char *tmpsecret = (char *)malloc(strlen(p_params->spsecret + 1));
		strcpy(tmpsecret, p_params->spsecret);
		char *tmpapp = (char *)malloc(strlen(p_params->spapplication + 1));
		strcpy(tmpapp, p_params->spapplication);
		*id = tmpid;
		*secret = tmpsecret;
		*app = tmpapp;
		syslog(LOG_INFO, "id: %s", tmpid);
		syslog(LOG_INFO, "secret: %s", tmpsecret);
		syslog(LOG_INFO, "app: %s", tmpapp);	
		rc = 0;
	}
	return rc;
}
