CROSS_COMPILE_ANDROID=	/opt/toolchains/arm-2009q3/bin/arm-none-linux-gnueabi-
CC=$(CROSS_COMPILE_ANDROID)gcc
CC=gcc
CFLAGS  = -g
LDLIBS+=parson/parson.c
RM      = rm -f


default: all

all: login 

login: login.c
	$(CC) $(CFLAGS)  -static -o login.cgi login.c cgic.h cgic.c 

#software test to test cross compiler
test: test.c
	$(CC) $(CFLAGS)  -static -o test.cgi test.c
clean :
	$(RM) *.cgi


