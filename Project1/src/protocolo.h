#pragma once

#include <sys/types.h>
#include <sys/stat.h>
#include <termios.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

#include "flags.h"



typedef struct {
    unsigned char flag ;
    unsigned char fieldA;
    unsigned char fieldC;
    unsigned char fieldBCC1;
    unsigned char* fieldD;
    unsigned char* fieldBCC2;
    
    //Other important variables
    unsigned int bcc2StufSize;
    unsigned int dataStufSize;
    size_t size_of_data_frame;
} DataStruct;

/**

* Function responsible for establishing logical connection between two points

Functionality diagram:


Sender                          Receiver


------>Send SET MESSAGE         
                            <-----Receive SET MESSAGE

                            <--------Send a UA confirmation
Receive UA <-------

CONNECTION ESTABLISHED


This function is protected by timeout alarm

* @param {...} port_number
* @param {TRANSMITTER/RECEIVER} flag
* @return {...} file descriptor on sucess, -1 otherwise
*/
int llopen(int port_number, int flag);

/**
*
LL WRITE IS THE MAIN API FUNCTION USED BY THE UPPER LAYER TO SEND THEIR MESSAGES TO THE OTHER MACHINE

LL WRITE IS ALSO PROTECTED BY A TIME OUT ALARM

1) MESSAGE IS SENT
2) CONFIRMATION WAITED
3) RR OR REJ COULD BE THE ANSWER.
IF RR SEND NEXT FRAME, IF REJ, ANTICIPATE TIMEOUT AND DO A RETRANSMISSION




* @param {file descriptor} fd
* @param {...} buffer
* @param {...} length
* @return {...} number of bytes written on success, 0 otherwise
*/
int llwrite(int fd, unsigned char * buffer, int length);

/**
 * LL READ IS THE API FUNCTION PROVIDED TO RECEIVE A MESSAGE FROM THE OTHER MACHINE
 * 
 * LL READ IS INDEED CONSTRUCTED IN ORDER TO TEST BCC VALUES AND TO DISCARD HEADERS IF NEEDED.
 * 
 * 
 * 
*
* @param {file descriptor} fd
* @param {...} buffer
* @return {...} buffer size on success, 0 therwise
*/
int llread(int fd, unsigned char * buffer);

/**
* LLCLOSE IS THE API FUNTION PROVIDED TO DISCONECT THE LOGICAL CHANNEL ESTABLISHED BY THE LLOPEN.

    AFTER LLCLOSE BOTH MACHINES ARE READY TO SHUTDOWN SAFELY THEIR COMUNICATION BRIDGE
*
*
* @param {file descriptor} fd
* @param {TRANSMITTER/RECEIVER} flag
* @return {...} 1 on sucsess, negativo value otherwise 
*/
int llclose(int fd, int flag);


