#include "general.h"

#define SERVER_NAME cgiServerName
#define _POSIX_SOURCE 1
#define FALSE 0
#define TRUE 1
#define MAX_LINE 200
char buf[1000];
#define MAX_MSG     200
#define PROP_DIR        ""
#define settings_PROP       PROP_DIR "settings.properties"
#define err_msg "error , wrong username or password"
#define ok_msg "ok"

int cgiMain()
{
	char username[20];
	char password[20];
	cgiFormStringNoNewlines("username", username, 20);
	cgiFormStringNoNewlines("password", password, 20);
	strcpy (buf,err_msg);
	if (strcmp(get_value (MAX_LINE,settings_PROP, "username",""),username)==0)
	{
		if (strcmp(get_value (MAX_LINE,settings_PROP, "userpw",""),password)==0)
		{
			strcpy(buf,ok_msg);
			cgiHeaderCookieSetInteger(COOKIE_LOGIN_STATUS, 1, COOKIE_EXPIRED_TIME, "/Nexion_design", SERVER_NAME);
		}
	}

	if (strcmp(get_value (MAX_LINE,settings_PROP, "adminname",""),username)==0)
	{
		if (strcmp(get_value (MAX_LINE,settings_PROP, "adminpw",""),password)==0)
		{
			strcpy(buf,ok_msg);
			cgiHeaderCookieSetInteger(COOKIE_LOGIN_STATUS, 2, COOKIE_EXPIRED_TIME, "/Nexion_design", SERVER_NAME);
		}
	}
	cgiHeaderContentType("text/html");
	fprintf(cgiOut, "%s",buf);
	return 0;
}


