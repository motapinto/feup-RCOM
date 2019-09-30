#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <termios.h>
#include <string.h>
#include <signal.h>

#include "flags.h"

/**
* ...description...
* @param {...} porta
* @param {...} flag
*/
int llopen(int porta, int flag);

/**
* ...description...
* @param {...} fd
* @param {...} buffer
* @param {...} length
*/
int llwrite(int fd, char * buffer, int length);

/**
* ...description...
* @param {...} fd
* @param {...} buffer
*/
int llread(int fd, char * buffer);

/**
* ...description...
* @param {...} fd
*/
int llclose(int fd);


/**
* ...description...
* @param {...} porta
* @param {...} buf
* @param {...} n_bytes
*/
int sendSet(int porta, unsigned char *buf,unsigned int n_bytes);

/**
* ...description...
* @param {...} signo
*/
void alarm_handler(int signo);