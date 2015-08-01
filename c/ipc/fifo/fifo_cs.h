#ifndef FIFO_CS_H
#define FIFO_CS_H

#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

#define MAX 25
#define SERVER_FIFO_NAME "SFIFO"
#define CLIENT_FIFO_NAME "C%d"

struct linking
{
	pid_t pid;
	char name[MAX];
};

#endif
