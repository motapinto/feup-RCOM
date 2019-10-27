#pragma once

//Port settings
#define BAUDRATE B38400
#define MODEMDEVICE_0 "/dev/ttyS0"
#define MODEMDEVICE_1 "/dev/ttyS1"
#define MODEMDEVICE_2 "/dev/ttyS2"
#define _POSIX_SOURCE 1 
#define FALSE 0
#define TRUE 1
#define UNKNOWN_PORT -1
#define OTHER_ERROR -2
#define LER_BYTE_A_BYTE 1

//LLOPEN
#define FLAG_LL_OPEN_TRANSMITTER 1
#define FLAG_LL_OPEN_RECEIVER 2

//LLCLOSE
#define FLAG_LL_CLOSE_TRANSMITTER_DISC 3
#define FLAG_LL_CLOSE_TRANSMITTER_UA 4
#define FLAG_LL_CLOSE_RECEIVER_DISC 5
#define FLAG_LL_CLOSE_RECEIVER_UA 6
#define FLAG_HANDLER_CALL 7
#define FLAG_LL_CLOSE_TRANSMITTER 8
#define FLAG_LL_DATA_SEND 9
#define FLAG_DATA_SEEKING_ANSWER_WITH1 10
#define FLAG_DATA_SEEKING_ANSWER_WITH0 11
#define FLAG_DATA_SENDING_ANSWER_RR_WITH1 12
#define FLAG_DATA_SENDING_ANSWER_RR_WITH0 13
#define FLAG_DATA_SENDING_ANSWER_REJ_WITH1 14
#define FLAG_DATA_SENDING_ANSWER_REJ_WITH0 15
#define LL_CLOSE_SUCESS 1
#define LL_CLOSE_FAIL -1

//5 Byte message
#define FLAG_INDEX_BEGIN 0
#define A_INDEX 1
#define C_INDEX 2
#define BCC_INDEX 3
#define FLAG_INDEX_END 4
#define START_INDEX 0

//Flags values
#define ESC 0x7d
#define ESC_ESC 0x5d
#define ESC_FLAG 0x5e
#define FLAG 0x7e
#define A_CE_AR 0x03
#define A_CR_AE 0x01
#define C(s) ((s == 1)? 0x40: 0x00)
#define C_SET 0x03
#define C_UA 0x07
#define C_DISC 0x0B
#define C_RR(r) ((r == 1)? 0x85: 0x05)
#define C_REJ(r) ((r == 1)? 0x81: 0x01)

//Other variables
#define BUF_SIZE 5
#define MAX_BUF 255
#define DATA_START_INDEX 4

//Return types
#define INVALID_PARAMS -1
#define READ_SUCCESS 0
#define READ_RR_SUCESS 4
#define READ_REJ_SUCESS 5
#define READ_FAIL -1
#define WRITE_SUCCESS 0
#define WRITE_FAIL 1
#define NO_MEM -2


//State machine flags
#define ST_START 0
#define ST_FLAG_RCV 1
#define ST_A_RCV 2
#define ST_C_RCV 3
#define ST_BCC_OK 4
#define ST_D 4
#define ST_ESC_RCV 5
#define ST_STOP 6
#define ST_C_RCV_RR 7
#define ST_C_RCV_REJ 8
#define ST_BCC_OK_RR 9
#define ST_BCC_OK_REJ 10

#define HANDLING_CLOSE_EMISSOR 0
#define HANDLING_CLOSE_RECETOR 1
#define HANDLING_UNDEFINED -1;
