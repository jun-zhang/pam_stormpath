#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>

#include <curl/curl.h>
#include <json/json.h>

#include "b64enc.h"
#include "spconfig.h"

#define SP_URL "https://api.stormpath.com/v1/applications/"
#define MAX_CREDS_LEN 1024
#define MAX_SPURL_LEN 4096
#define MAX_AUTHC_LEN 1024
#define MAX_AUTH_DATA_LEN 2048

int auth_check(void *json_ptr, size_t total_size)
{
	openlog("pam_stormpath", 0, LOG_AUTHPRIV);
	char *json = malloc(total_size * 2);
	struct json_object *new_obj;
	strcpy(json, (char *) json_ptr);
	new_obj = json_tokener_parse(json);
	syslog(LOG_INFO, "%s", json_object_to_json_string(new_obj));
	json_object_put(new_obj);
	return 0;
}

static size_t sp_reader(void *ptr, size_t size, size_t nmemb, void *userdata)
{
	size_t total_size = nmemb * size;
	auth_check(ptr, total_size);
	return total_size;
}

int syslog_trace(CURL *handle, curl_infotype type,
		char *data, size_t size,
		void *userp)
{
	char *text = malloc(strlen(data) + 1);
	strcpy(text, data);
	(void)handle;

	syslog(LOG_INFO, "curl debug_data: %s", text);
	free(text);
	return 0;
}

int make_req(char *username, char *password)
{
	const char SPID[] = "SP_ID";
	const char SPSECRET[] = "SP_SECRET";
	const char SPAPPLICATION[] = "SP_APPLICATION";
	const char LOGINATTEMPTS[] = "/loginAttempts";

	char *spid = NULL;
	char *spsecret = NULL;
	char *spapplication = NULL;

	sp_config(&spid, &spsecret, &spapplication);


	openlog("pam_stormpath", 0, LOG_AUTHPRIV);

	if ( (spid == NULL) || (spsecret == NULL) || (spapplication== NULL) ){
		syslog(LOG_INFO, "You must set the environment variables: %s, %s and %s\n", SPID, SPSECRET, SPAPPLICATION);
		return 1;
	}
	
	syslog(LOG_INFO, "spid: %s", spid);
	syslog(LOG_INFO, "spsecret: %s", spsecret);
	syslog(LOG_INFO, "spapplication: %s", spapplication);

        char creds[MAX_CREDS_LEN];
	snprintf(creds, (size_t) MAX_CREDS_LEN - 1, "%s:%s", spid, spsecret);
	syslog(LOG_INFO, "Creds: %s", creds);

        char spurl[MAX_SPURL_LEN];
        snprintf(spurl, (size_t) MAX_SPURL_LEN - 1, "%s%s%s", SP_URL, spapplication, LOGINATTEMPTS);
	syslog(LOG_INFO, "URL: %s", spurl);

	char auth_clear[MAX_AUTHC_LEN];
	snprintf(auth_clear, (size_t) MAX_AUTHC_LEN, "%s:%s", username, password);

        char *auth_b64;
	b64enc(&auth_b64, auth_clear);
	syslog(LOG_INFO, "auth_clear: %s", auth_clear);
	syslog(LOG_INFO, "auth_b64: %s", auth_b64);

	char pre_json[] = "{\"type\":\"basic\",\"value\":\"";
	char post_json[] = "\"}";
        char auth_data[MAX_AUTH_DATA_LEN];
        snprintf(auth_data, (size_t) MAX_AUTH_DATA_LEN, "%s%s%s", pre_json, auth_b64, post_json);

	syslog(LOG_INFO, "auth_data: %s", auth_data);
	syslog(LOG_INFO, "setting up curl");
	CURL *c;
	CURLcode res;
	struct curl_slist *headers = NULL;
	headers = curl_slist_append(headers, "Accept: application/json");
	headers = curl_slist_append(headers, "Content-Type: application/json");

	CURLcode curl_rc;
	curl_rc = curl_global_init(CURL_GLOBAL_ALL);
	syslog(LOG_INFO, "curl init rc: %d", curl_rc);
	if (curl_rc != CURLE_OK)
	{
		syslog(LOG_INFO, "problem initializing curl: %d", curl_rc);
	}
	c = curl_easy_init();
	curl_easy_setopt(c, CURLOPT_VERBOSE, 1L);
	curl_easy_setopt(c, CURLOPT_DEBUGFUNCTION, syslog_trace);
	curl_easy_setopt(c, CURLOPT_NOPROGRESS, 1L);
	curl_easy_setopt(c, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
	curl_easy_setopt(c, CURLOPT_USERPWD, creds);
	curl_easy_setopt(c, CURLOPT_URL, spurl);
	curl_easy_setopt(c, CURLOPT_POST, 1L);
	curl_easy_setopt(c, CURLOPT_POSTFIELDS, auth_data);
	curl_easy_setopt(c, CURLOPT_POSTFIELDSIZE, strlen(auth_data));
	curl_easy_setopt(c, CURLOPT_HTTPHEADER, headers);
	curl_easy_setopt(c, CURLOPT_WRITEFUNCTION, sp_reader);
	syslog(LOG_INFO, "Curl opts set");
	long http_code = 0;
	res = curl_easy_perform(c);
	if (res != CURLE_OK)
	{
		syslog(LOG_INFO, "curl failed: %s\n", curl_easy_strerror(res));
	}
	curl_easy_getinfo(c, CURLINFO_RESPONSE_CODE, &http_code);
	int rc = 0;
	if (http_code == 200 && res != CURLE_ABORTED_BY_CALLBACK)
	{
		rc = 0;
	} else {
		rc = 1;
	}
        free(auth_b64);
        free(spapplication);
        free(spsecret);
        free(spid);
	curl_slist_free_all(headers);
	curl_easy_cleanup(c);
	return rc;
}
