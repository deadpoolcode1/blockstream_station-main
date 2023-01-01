#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "cgic.h"
#include <errno.h>
#include<sys/socket.h>
#include<arpa/inet.h> 
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <termios.h> 
#include<syslog.h>

#define SERVER_NAME cgiServerName
#define SAVED_ENVIRONMENT "/tmp/cgicsave.env"

#define COOKIE_USER_NAME	  "cookie_user_name"
#define COOKIE_USER_PW		  "cookie_user_pw"
#define COOKIE_ADMIN_NAME   "cookie_admin_name"
#define COOKIE_ADMIN_PW     "cookie_admin_pw"
#define COOKIE_LOGIN_STATUS "cookie_login_status"
#define COOKIE_EXPIRED_TIME 30 * 60
#define driver_ip "10.0.0.9"
#define driver_socket 27015
#define BUF_SIZE 1024
#define short_resp 500
#define medium_resp 3000
#define mpu9250_gyro_resp 4000
void CookieSet();
void Cookies();
char* fget_contents(char* filename);
void system_command_root(const char *command);
char * system_command(const char *command, char* reply);
int system_command_write(char* command);
char* trim_sring_at_str ( char *s, char const *sub );
char* trim_sring_at_str2 ( char *s, char const *sub );
int char_exists (char* big_string,char char_to_find);
int my_strstr( char const *s, char const *sub );
char* myitoa(int value, char* str, int radix);
char *get_value (int MAX_LINE,const char* config_file, const char* name,char* ret_buf);
char* find_char_or_comment(const char* s, char c);
char* lskip(const char* s);
char* rstrip(char* s);
void get_line (int MAX_LINE,const char* file_name, const char* name ,char* line_value );
void extract(char *s,char *t,char *d,int pos,int len);
void logmsg (const char *msg, int err, int level);
char * substr(char * s, int x, int y);
static int set_value (int MAX_LINE,const char* config_file, const char* name ,const char* new_value );
int send_socket (const char* socket_command);
int read_file_to_buffer (const char* file_name, char* buffer_name,int length);
char* read_rs232(char* port, int baud, char* data,int time);
char* read_rs232e(char* port, int baud, char* data,int time);
int open_port( char* serial_port);
char file_ex_data [80];
int check_snprintf(int ret_value, int max_length);

int open_port( char* serial_port)
{
  int fd; /* File descriptor for the port */
  fd = open(serial_port, O_RDWR | O_NOCTTY | O_NDELAY);
  if (fd == -1)
      printf ( "unable to open port %s\n", serial_port);
  else
    fcntl(fd, F_SETFL, FNDELAY);
  return (fd);
}
char* read_rs232(char* port, int baud_rate, char* data,int time)
{
  int fd; 
  speed_t baud;
  fd = open_port(port);
  // Read the configureation of the port
  struct termios options;
  tcgetattr( fd, &options );
  /* SEt Baud Rate */
  if (baud_rate==115200)
  baud = B115200;
  if (baud_rate==38400)
  baud= B38400;
  if (baud_rate==9600)
  baud=B9600;
  cfsetispeed( &options, baud );
  cfsetospeed( &options, baud );
  options.c_cflag |= ( CLOCAL | CREAD ); 
  options.c_cflag &= ~CSIZE; 
  options.c_cflag |= CS8;      
  options.c_cflag &= ~PARENB;
  options.c_cflag &= ~CSTOPB;
  options.c_cflag &= ~CSIZE;
  options.c_cflag |= CS8;
  options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
  options.c_iflag &= ~(IXON | IXOFF | IXANY);
  options.c_oflag &= ~OPOST;
  if ( tcsetattr( fd, TCSANOW, &options ) == -1 )
    printf ("Error with tcsetattr = %s\n", strerror ( errno ) );
  fcntl(fd, F_SETFL, FNDELAY); 
  write (fd, data, strlen(data));           // send 7 character greetin
  data[0]='\0';
  write (fd, "\r", 1);           // send characters
  usleep ((7 + 25) *time);             // sleep enough to transmit small data plus
  data[0]='\0';
  read (fd, data,1000);  // read up to 1000 characters if ready to read
  close(fd);
  return data;	
}
char* read_rs232e(char* port, int baud_rate, char* data,int time)
{
  int fd;
  int i=0;
  char *pos;
  speed_t baud;
  fd = open_port(port);
  // Read the configureation of the port
  struct termios options;
  tcgetattr( fd, &options );
  /* SEt Baud Rate */
  if (baud_rate==115200)
  baud = B115200;
  if (baud_rate==38400)
  baud= B38400;
  if (baud_rate==9600)
  baud=B9600;
  cfsetispeed( &options, baud );
  cfsetospeed( &options, baud );
  options.c_cflag |= ( CLOCAL | CREAD );
  options.c_cflag &= ~CSIZE;
  options.c_cflag |= CS8;
  options.c_cflag &= ~PARENB;
  options.c_cflag &= ~CSTOPB;
  options.c_cflag &= ~CSIZE;
  options.c_cflag |= CS8;
  options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
  options.c_iflag &= ~(IXON | IXOFF | IXANY);
  options.c_oflag &= ~OPOST;
  if ( tcsetattr( fd, TCSANOW, &options ) == -1 )
    printf ("Error with tcsetattr = %s\n", strerror ( errno ) );
  fcntl(fd, F_SETFL, FNDELAY);
  write (fd, data, strlen(data));           // send 7 character greetin
  data[0]='\0';
  write (fd, "\r", 1);           // send characters
  usleep ((7 + 25) *time);             // sleep enough to transmit small data plus
  data[0]='\0';
  read (fd, data,1000);  // read up to 1000 characters if ready to read
  close(fd);
  trim_sring_at_str2 ( data, "root@" );
  return data;
}

char* myitoa(int value, char* str, int radix)
{
    static char dig[] =
        "0123456789"
        "abcdefghijklmnopqrstuvwxyz";
    int n = 0, neg = 0;
    unsigned int v;
    char* p, *q;
    char c;

    if (radix == 10 && value < 0) {
        value = -value;
        neg = 1;
    }
    v = value;
    do {
        str[n++] = dig[v%radix];
        v /= radix;
    } while (v);
    if (neg)
        str[n++] = '-';
    str[n] = '\0';

    for (p = str, q = p + (n-1); p < q; ++p, --q)
        c = *p, *p = *q, *q = c;
    return str;
}
int my_strstr( char const *s, char const *sub )
{
	char *pch = strstr(s, sub);
	if (pch) 
		{
		   int index = pch - s;
			return index;
		}
	else return 0;
}
void extract(char *s,char *t,char *d,int pos,int len)
{
	s=s+(pos-1);
	t=s+len;
	while(s!=t)
	{
		*d=*s;
		s++;
		d++;
	}
		*d='\0';
}
void get_line (int MAX_LINE,const char* file_name, const char* name ,char* line_value )
{
    char line[MAX_LINE];
    int i;
    FILE* file;
    if ((file = fopen (file_name, "r+")) == NULL) {
        goto nofile;
    }
   file_ex_data[0]='\0';
    while (fgets (line, sizeof line, file) != NULL)
    {
	if (strncmp(line,name,strlen(name))==0)
	{
		for (i=0;i<strlen(line);i++)
		file_ex_data[i]=line[i];

	}
    }
    fclose (file);

nofile:;
}
/* Strip whitespace chars off end of given string, in place. Return s. */
char* rstrip(char* s)
{
    char* p = s + strlen(s);
    while (p > s && isspace(*--p))
        *p = '\0';
    return s;
}
char* fget_contents(char* filename)
{
FILE *f = fopen(filename, "rb");
fseek(f, 0, SEEK_END);
long fsize = ftell(f);
fseek(f, 0, SEEK_SET);
char *string = malloc(fsize + 1);
fread(string, fsize, 1, f);
fclose(f);
string[fsize] = 0;
return string;
}
/* Return pointer to first non-whitespace char in given string. */
char* lskip(const char* s)
{
    while (*s && isspace(*s))
        s++;
    return (char*)s;
}
/* Return pointer to first char c or '#' in given string, or pointer to
   null at end of string if neither found. */
char* find_char_or_comment(const char* s, char c)
{
    while (*s && *s != c && *s != '#')
        s++;
    return (char*)s;
}
/* Parse config_file. Return a pointer to the value of 'name'.
 * If 'name' is not found, return a pointer to an empty string.
 */
char *get_value (int MAX_LINE,const char* config_file, const char* name,char* ret_buf)
{
    char line[MAX_LINE];
    FILE* file;

    char* start;
    char* end;
    int found = 0;

    if ((file = fopen (config_file, "r")) == NULL) {
        goto nofile;
    }

    while (fgets (line, sizeof line, file) != NULL) {
        start = lskip (rstrip (line));

        if (*start == '\0' || *start == '#') /* ignore comments */
            continue;

        end = find_char_or_comment (start, '=');
        if (*end != '=') /* no '=' found, ignore */
            continue;

        *end = '\0';
        rstrip (start);
        if (strcmp (name, start) != 0) /* name doen't match, continue */
            continue;

        /* match found */
        found = 1;
        start = lskip (end + 1);
        end = find_char_or_comment (start, '#');
        if (*end == '#')
            *end = '\0';
        rstrip (start);
        break;
    }

    fclose (file);

nofile:
    if (found)
        return start;
    else {
        line[0] = '\0';
        ret_buf[0] = '\0';
        strcpy(ret_buf,line);
        return ret_buf;
    }
}
int char_exists (char* big_string,char char_to_find)
{
	char * general_pointer;
	general_pointer=strchr(big_string,char_to_find);
	if (general_pointer!=NULL)
	return (1);
	else return (0);
}

void system_command_root(const char *command)
{
	int fd[2];
	int childpid;
	pipe(fd);
	if ( (childpid = fork() ) == -1){
   		fprintf(stderr, "FORK failed");
   		return;
	} else if( childpid == 0) {
   		close(1);
   		dup2(fd[1], 1);
   		close(fd[0]);
   		execlp("/bin/sh","/bin/sh","-c",command,NULL);
	}
}

char* system_command(const char *command, char* reply)
{
    FILE *fd;
    fd = popen(command, "r");
    if (!fd) return NULL;
 
    char   buffer[256];
    size_t chread;
    /* String to store entire command contents in */
    size_t comalloc = 256;
    size_t comlen   = 0;
    char  *comout   = malloc(comalloc);
 
    /* Use fread so binary data is dealt with correctly */
    while ((chread = fread(buffer, 1, sizeof(buffer), fd)) != 0) {
      if (comlen + chread >= comalloc) {
          comalloc *= 2;
          comout = realloc(comout, comalloc);
      }
      memmove(comout + comlen, buffer, chread);
      comlen += chread;
    }
 
    memcpy(reply, comout, comlen * sizeof(char));
    free(comout);
    pclose(fd);
    return reply;
}

int system_command_write(char* command)
{
	int status=0;
   	system(command);
	return status;
}
char* trim_sring_at_str ( char *s, char const *sub )
{
 int location_to_trim=0;
 location_to_trim=my_strstr(s,sub);
 if (location_to_trim>0)
 	s[location_to_trim+1]='\0';
 return s;
}
char* trim_sring_at_str2 ( char *s, char const *sub )
{
 int location_to_trim=0;
 location_to_trim=my_strstr(s,sub);
 location_to_trim--;
 if (location_to_trim>0)
        s[location_to_trim+1]='\0';
 return s;
}

char * substr(char * s, int x, int y)
{
    char * ret = malloc(strlen(s) + 1);
    char * p = ret;
    char * q = &s[x];


    while(x  < y)
    {
        *p++ = *q++;
        x ++;
    }

    *p++ = '\0';

    return ret;
}
int read_file_to_buffer (const char* file_name, char* buffer_name,int length )
{
    int fd;
    fd = open(file_name, O_RDWR);
    if ( fd < 0) 
    {
        fputs("Error while opening", stderr);
        return 1;
    }
    read(fd,buffer_name,length);
    close(fd);   
    return 0;
}
static int set_value (int MAX_LINE,const char* config_file, const char* name ,const char* new_value )
{
    char line[MAX_LINE];
    static char write_file_data[1000];
    FILE* file;

    char* start;
    int found = 0;
    write_file_data[0]='\0';
    if ((file = fopen (config_file, "r+")) == NULL) {
        goto nofile;
    }
    while (fgets (line, sizeof line, file) != NULL)
    {
	if (strncmp(line,name,strlen(name))==0)
	{
		line[0]='\0';
		strcat (line,name);
		strcat (line,"=");
		strcat (line,new_value);
		strcat (line,"\n");
	}
	strcat (write_file_data,line);
    }
    fclose (file);

    if ((file = fopen (config_file, "r+")) == NULL) {
        goto nofile;
    }
    fputs (write_file_data, file);
    fclose (file);
nofile:
    if (found)
        return 0;
    else {
        line[0] = '\0';
        return 0;
    }
}
int send_socket(const char* socket_command)
{

    int socket_desc;
    struct sockaddr_in server;
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        return (-1);
    }

    server.sin_addr.s_addr = inet_addr(driver_ip);
    server.sin_family = AF_INET;
    server.sin_port = htons( driver_socket );

    //Connect to remote server
    if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
	close(socket_desc);
        return (-1);
    }
    if( write(socket_desc,socket_command,strlen(socket_command)) < 0)
    {
        close(socket_desc);
        return (-1);
    }
    //read (input_fd,socket_command, BUF_SIZE);
    close(socket_desc);
    return 0;

}

void CookieSet()
{
	char cname[1024];
	char cvalue[1024];
	/* Must set cookies BEFORE calling cgiHeaderContentType */
	cgiFormString("cname", cname, sizeof(cname));
	cgiFormString("cvalue", cvalue, sizeof(cvalue));
	if (strlen(cname)) {
		/* Cookie lives for one day (or until browser chooses
			to get rid of it, which may be immediately),
			and applies only to this script on this site. */
		cgiHeaderCookieSetString(cname, cvalue,
			86400, cgiScriptName, SERVER_NAME);
	}
}

void LoadEnvironment()
{
	if (cgiReadEnvironment(SAVED_ENVIRONMENT) !=
		cgiEnvironmentSuccess)
	{
		cgiHeaderContentType("text/html");
		fprintf(cgiOut, "<head>Error</head>\n");
		fprintf(cgiOut, "<body><h1>Error</h1>\n");
		fprintf(cgiOut, "cgiReadEnvironment failed. Most "
			"likely you have not saved an environment "
			"yet.\n");
		exit(0);
	}
	/* OK, return now and show the results of the saved environment */
}

void SaveEnvironment()
{
	if (cgiWriteEnvironment(SAVED_ENVIRONMENT) !=
		cgiEnvironmentSuccess)
	{
		fprintf(cgiOut, "<p>cgiWriteEnvironment failed. Most "
			"likely %s is not a valid path or is not "
			"writable by the user that the CGI program "
			"is running as.<p>\n", SAVED_ENVIRONMENT);
	} else {
		fprintf(cgiOut, "<p>Environment saved. Click this button "
			"to restore it, playing back exactly the same "
			"scenario: "
			"<form method=POST action=\"");
		cgiValueEscape(cgiScriptName);
		fprintf(cgiOut, "\">"
			"<input type=\"submit\" "
			"value=\"Load Environment\" "
			"name=\"loadenvironment\"></form><p>\n");
	}
}

/** @brief checks sprintf return function, prints and logs accordinglly.
 */
int check_snprintf(int ret_value, int max_length)
{
        if (ret_value < 0)
                printf ("error, invalid sprintf action \n");
        else if (ret_value >= max_length)
                printf("error, string truncated \n");
        else
                return 0;
        return -1;
}
