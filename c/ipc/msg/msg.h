#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>

#include <fcntl.h>

#define MAX_TEXT 512 
struct my_msg_st {
	long int my_msg_type;
	char some_text[MAX_TEXT];
};


