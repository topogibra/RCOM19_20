/*Non-Canonical Input Processing*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include "state_machine.h"

#define BAUDRATE B9600
#define _POSIX_SOURCE 1 /* POSIX compliant source */
#define FALSE 0
#define TRUE 1

volatile int stop_var= 0;

int main(int argc, char** argv)
{
    int fd,c, res;
    struct termios oldtio,newtio;
    char result[255];

    if ( (argc < 2) || 
  	     ((strcmp("/dev/ttyS0", argv[1])!=0) && 
  	      (strcmp("/dev/ttyS1", argv[1])!=0) && 
          (strcmp("/dev/ttyS2", argv[1])!=0) )) {
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
    newtio.c_cc[VMIN]     = 1;   /* blocking read until 5 chars received */



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

    states state_machine = START;


    for (int i = 0; stop_var == 0; i++) {
        res = read(fd, &result[i], 1);
        if (result[i] == '\0') stop_var = 1;	
        else {
            advance_state_UA(result[i], state_machine);
        }
    } 	

    printf("%s\n",result);

  /* 
    O ciclo WHILE deve ser alterado de modo a respeitar o indicado no gui�o 
  */

    sleep(2);

    res = write(fd,result,strlen(result)+1);
    printf("%d bytes written.\n",res);

    sleep(1);

    tcsetattr(fd,TCSANOW,&oldtio);
    close(fd);
    return 0;
}
