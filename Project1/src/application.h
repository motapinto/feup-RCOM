#pragma once

typedef unsigned int uint;
typedef unsigned char uchar;

typedef enum {
    Data = 1, 
    Start = 2, 
    End = 3
} ControlField; 

typedef enum {
    FileSize = 1, 
    FileName = 2, 
} Type; 

typedef enum {
    Receiver = 0, 
    Sender = 1, 
} machineType; 

typedef struct {
    Type type;
    uchar length;
    uchar value[MAX_BUF];
} TLV;

typedef struct {
    ControlField fieldC;
    uchar fieldN;
    uchar fieldL2;
    uchar fieldL1;
    uchar fieldP[MAX_BUF];
} AppDataStruct;

typedef struct {
    ControlField fieldC;
    TLV fileSize;
    TLV fileName;
} AppControlStruct;


/**
* Assemble and send data block
* @param {file descriptor} fd
* @param {sequence number (%255)} sequenceNumber
* @param {data} buffer
* @param {data length} length
* @return {...} 0 on success, -1 otherwise
*/
int sendDataBlock(int fd, uint sequenceNumber, uchar *buffer, uint length);

/**
* Receives data block and processes it
* @param {file descriptor} fd
* @param {returns sequence number (%255)} sequenceNumber
* @param {returns data block received} buffer
* @return {...} length of block received on success, -1 otherwise
*/
int receiveDataBlock(int fd, uint *sequenceNumber, uchar *buffer);

/**
* Assemble and send control block
* @param {file descriptor} fd
* @param {control field(START/END)} fieldC
* @param {TLV first message} fileSize
* @param {TLV second message} fileName
* @return {...} 0 on success, -1 otherwise
*/
int sendControlBlock(int fd, int fieldC, uint fileSize, char *fileName);

/**
* Receives control block and processes it
* @param {file descriptor} fd
* @param {returns control field(START/END)} type
* @param {returns fileName} fileName
* @return {...} FileSize on success, -1 otherwise
*/
int receiveControlBlock(int fd, uint *type, char *fileName);
