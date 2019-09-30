#pragma once

#define FLAG_LL_OPEN_TRANSMITTER 1
#define FLAG_LL_OPEN_RECEIVER 2

//5 Byte message
#define FLAG_INDEX_BEGIN 0
#define A_INDEX 1
#define C_INDEX 2
#define BCC_INDEX 3
#define FLAG_INDEX_END 4

//Flags values
#define FLAG 0x99
#define A_EM 0x03 
#define A_RE 0x01
#define C_SET 0x03
#define C_UA 0x07

//Other variables
#define BUF_SIZE 5
