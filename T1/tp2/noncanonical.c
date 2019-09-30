/*Non-Canonical Input Processing*/

#include <sys/types.h>
#include <sys/stat.h>
#include <termios.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>

#include "protocolo.h"

#define ST_START 0
#define ST_FLAG_RCV 1
#define ST_A_RCV 2
#define ST_C_RCV 3
#define ST_BCC_OK 4
#define ST_STOP 5
#define READ_FAIL 1
#define READ_SUCCESS 0

#define BAUDRATE B38400
#define _POSIX_SOURCE 1 /* POSIX compliant source */
#define FALSE 0
#define TRUE 1

int receive_SET(int fd){
  unsigned char buf[255];
  unsigned int size = 0, state = ST_START;
  for (size = 0; state != ST_STOP; size++){
    //read byte
    if (!read(fd,&buf[size],1)){
      perror("Failled to read");
      return READ_FAIL;
    }

    switch (state){
      case ST_START:
        //check FLAG byte
        if(buf[size] == FLAG)
          state = ST_FLAG_RCV;
        break;

      case ST_FLAG_RCV:
        switch (buf[size]){
          case A_EM:
            //received A, go to state A received
            state = ST_A_RCV;
            break;
          
          case FLAG:
            //received FLAG, same state
            break;
          
          default:
            //received other, go to start
            state = ST_START;
            break;
        }
        break;

      case ST_A_RCV:
        switch (buf[size]){
          case C_SET:
            //received C, go to state C received
            state = ST_C_RCV;
            break;
          
          case FLAG:
            //received FLAG, go to flag state
            state = ST_FLAG_RCV;
            break;
          
          default:
            //received other, go to start
            state = ST_START;
            break;
        }
        break;

      case ST_C_RCV:
        //received BCC, check BCC
        if(buf[size] == (A_EM^C_SET))
          //BCC correct
          state = ST_BCC_OK;
        else if(buf[size] == FLAG)
          //Received FLAG
          state = ST_FLAG_RCV;
        else
          //Received other
          state = ST_START;
        break;

      case ST_BCC_OK:
          //check FLAG byte
          if(buf[size] == FLAG)
            //received all, stop cycle
            return READ_SUCCESS;
          else 
            //received other, go to start
            state = ST_START;
        break;

      default:
        state = ST_START;
        break;
    }
  }

  return READ_SUCCESS;
}


volatile int STOP=FALSE;

int main(int argc, char** argv)
{
    int fd,c, res;
    struct termios oldtio,newtio;
    unsigned char buf[255];
    char unit_char_received[2]; //2 positions because of
    int size=0;

    if ( (argc < 2) || 
  	     ((strcmp("/dev/ttyS0", argv[1])!=0) && 
  	      (strcmp("/dev/ttyS1", argv[1])!=0) )) {
      printf("Usage:\tnserial SerialPort\n\tex: nserial /dev/ttyS1\n");
      exit(1);
    }


  /*
    Open serial port device for reading and writing and not as controlling tty
    because we don't want to get killed if linenoise sends CTRL-C.
  */
  
    
    fd = open(argv[1], O_RDWR | O_NOCTTY );
    if (fd <0) {perror(argv[1]); exit(-1); }

    if ( tcgetattr(fd,&oldtio) == -1) { /* save current port settings */
      perror("tcgetattr");
      exit(-1);
    }

    bzero(&newtio, sizeof(newtio));
    newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;
    newtio.c_iflag = IGNPAR;
    newtio.c_oflag = 0;

    /* set input mode (non-canonical, no echo,...) */
    newtio.c_lflag = 0;

    newtio.c_cc[VTIME]    = 0;   /* inter-character timer unused */
    newtio.c_cc[VMIN]     = 5;   /* blocking read until 5 chars received */



  /* 
    VTIME e VMIN devem ser alterados de forma a proteger com um temporizador a 
    leitura do(s) pr�ximo(s) caracter(es)
  */



    tcflush(fd, TCIOFLUSH);

    if ( tcsetattr(fd,TCSANOW,&newtio) == -1) {
      perror("tcsetattr");
      exit(-1);
    }

    printf("New termios structure set\n");



    /*    
    size = 0;
    //Receving loop
    while (STOP==FALSE) {       //loop for input 
      
      res = read(fd,&buf[size],1);   //read one char
      if(res==-1){
        printf("ERRORROROORORRO\n");
        return -1;
      }

    
      if (buf[size]=='*'){
        STOP=TRUE;
        buf[size] = 0;
        printf(":%s:%d\n", buf, size+1);
      }

      size+=res;
  
    }
    */

    sleep(1); //Resolve bug de receber lixo
    tcsetattr(fd,TCSANOW,&oldtio);
    close(fd);
    return 0;
}
