#include "protocolo.h"
#include "application.h"
#include "files.h"

int sendFile(int portNumber, char *fileName) {
    uint size_file;
    FILE* file = fopen(fileName, "rb");
    if(file == NULL) {
        printf("Could not open %s!\n", fileName);
        return -1;
    }
    
    int fd = llopen(portNumber, FLAG_LL_OPEN_TRANSMITTER);
    if(fd < 0) {
        printf("Error in llopen!\n");
        return -1;
    }

    size_file = fileSize(file);
    // Send control package - START
    if(sendControlBlock(fd, Start, size_file, fileName) < 0) {
        printf("Error in sendControlBlock!\n");
        return -1;
    }

    uint length, nBytes = 0, sequenceNumber = 0;
    uchar buffer[MAX_BUF];

    // While reads file sendDataPackage
    while(nBytes != size_file) {
        length = fread(buffer, sizeof(char), MAX_BUF, file);
        if(length < 0){
            perror("Failled to read from file");
            return -1;
        }

        if(sendDataBlock(fd, sequenceNumber%255, buffer, length) == EOF) {
            printf("Error in sendDataPackage!\n");
            return -1;
        }
        sequenceNumber++;
        nBytes += length;
    }

    if(fclose(file) != 0) {
        printf("Error while closing file!\n");
        return -1;
    }

    // Send control package - END
    if(sendControlBlock(fd, End, size_file, fileName) < 0) {
        printf("Error in sendControlBlock!\n");
        return -1;
    }

    if(llclose(fd, FLAG_LL_CLOSE_TRANSMITTER_DISC) != LL_CLOSE_SUCESS) {
        printf("Error in llclose!\n");
        return -1;
    }

    return nBytes;
}

int receiveFile(int portNumber) {
    int fd = llopen(portNumber, FLAG_LL_OPEN_RECEIVER);
    if(fd < 0) {
        printf("Error in llopen!\n");
        return -1;
    }

    char fileName[MAX_BUF];
    uint fileSize = 0, controlType;

    // Receive control block - START
    if((fileSize = receiveControlBlock(fd, &controlType, fileName)) < 0) {
        printf("Error in sendControlBlock!\n");
        return -1;
    }

    if(controlType != Start) {
        printf("controlType value is not START\n");
        return -1;
    }

    //w: Create if does not exist / erase if exists
    FILE* file = fopen(fileName, "w");
    if(file == NULL) {
        printf("Could not open/create %s!\n", fileName);
        return -1;
    }

    uint sequenceNumber, length = 0, totalLength = 0;
    uchar buffer[MAX_BUF];

    while(totalLength != fileSize) {
        // Receive data block
        if((length = receiveDataBlock(fd, &sequenceNumber, buffer)) <= 0) {
            printf("Error in receiveDataBlock!\n");
            return -1;
        }

        totalLength += length;

        // Writes data block
        if(fwrite(buffer, sizeof(char), length, file) != length) {
            printf("Error while writing to output file!\n");
            return -1;
        }

    }

    if(fclose(file) != 0) {
        printf("Error while closing file!\n");
        return -1;
    }

    // Receive control block - END
    if(receiveControlBlock(fd, &controlType, fileName) < 0) {
        printf("Error in receiveControlBlock!\n");
        return -1;
    }

    if(controlType != End) {
        printf("controlType value is not END\n");
        return -1;
    }

    if(llclose(fd, FLAG_LL_CLOSE_RECEIVER_DISC) != LL_CLOSE_SUCESS) {
        printf("Error in llclose!\n");
        return -1;
    }

    return 0;
}

uint fileSize(FILE *fp) {
    uint counter = 0;

    if(fp == NULL) {
        printf("File pointer is NULL!\n");
        return -1;
    }

    if(fseek(fp, 0, SEEK_END) < 0) {
        printf("Error in fseek!\n");
        return -1;
    }

    if((counter = ftell(fp)) < 0) {
        printf("Error in ftell!\n");
        return -1;
    }

    if(fseek(fp, 0, SEEK_SET) < 0) {
        printf("Error in fseek!\n");
        return -1;
    }

    return counter;
}