/*WRITE Non-Canonical Input Processing*/
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>
#include "llmacros.h"
#include "interface.h"
#include "applicationLayer.h"
#include <time.h>

int main(int argc, char **argv)
{
  if ((argc < 2) || (atoi(argv[1]) < 0 || atoi(argv[1]) > 2))
  {
    printf("Usage:\tnserial SerialPort\n\tex: nserial /dev/ttySx\n");
    exit(1);
  }
  clock_t t;
  t = clock();
  applicationLayerSender(atoi(argv[1]), argv[2]);
  t = clock() - t;

  double time_taken = ((double)t) / CLOCKS_PER_SEC;
  printf("Transfer elapsed time: %f seconds\n", time_taken);
  return 0;
}
