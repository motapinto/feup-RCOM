#include "protocolo.h"
#include <signal.h>

#define MAX_RETR 3
#define TIMEOUT 3

bool finish = false;
int retry_num = 0;
int porta = 0;
unsigned char *buf = NULL;
unsigned int n_bytes = 0;

void alarm_handler(int signo) {

  while(!finish) {
    
    if(retry_num < MAX_RETR) {
      if(sendSet(porta, buf, n_bytes) == -1) {
        return;
      }

      alarm(TIMEOUT);
      retry_num++;
    }

    else {
      finish = true;
    }
  }


  return;
}

int sendSet(int porta, unsigned char *buf, unsigned int n_bytes) {

  int bytes_send = write(porta, buf, n_bytes);

  if(bytes_send != BUF_SIZE) {
    perror("Error writing in llopen:");
    return -1;
  }

  return bytes_send;
}


int llopen(int porta, int flag) {

  unsigned char buf[5];

  if(flag == FLAG_LL_OPEN_TRANSMITTER) {
    buf[C_INDEX] = C_SET;
  }

  else if(flag == FLAG_LL_OPEN_RECEIVER) {
    buf[C_INDEX] = C_UA;
  }

  else {
    printf("Wrong flag value\n");
    return -1;
  }

  buf[FLAG_INDEX_BEGIN] = FLAG;
  buf[A_INDEX] = A_EM;
  buf[BCC_INDEX] = buf[A_INDEX] ^ buf[C_INDEX];
  buf[FLAG_INDEX_END] = FLAG;

  //Sending SET/UA frame
  if(flag == FLAG_LL_OPEN_TRANSMITTER) {

    if(sendSet(porta, buf, sizeof(buf)) != BUF_SIZE) {
      printf("Error in sendSet function\n");
    }

    if(signal(SIGALRM, alarm_handler) == SIG_ERR) {
      perror("Error instaling SIG ALARM handler\n");
      return -1;
    }

    if(read(porta, buf, sizeof(buf)) != BUF_SIZE) {
      perror("Error reading from llopen\n");
      return -1;
    }

    if(signal(SIGALRM, SIG_IGN) == SIG_ERR) {
      perror("Error in ignoring SIG ALARM handler");
    }
  }
  
  //Receives  SET/UA frame
  else if(flag == FLAG_LL_OPEN_RECEIVER) {

    if(read(porta, buf, sizeof(buf)) != BUF_SIZE) {
      perror("Error in reading from llopen:");
      return -1;
    }

    if(signal(SIGALRM, SIG_IGN) == SIG_ERR) {
      perror("Error in ignoring SIG ALARM handler");
    }

    if(write(porta, buf, sizeof(buf)) != BUF_SIZE) {
      perror("Error in writing from llopen:");
      return -1;
    }

    if(signal(SIGALRM, alarm_handler) == SIG_ERR) {
      perror("Error instaling SIG ALARM handler\n");
      return -1;
    }
  }
  return porta;
}
