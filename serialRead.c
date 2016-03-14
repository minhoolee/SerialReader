/* www.chrisheydrick.com
     
   June 23 2012

   CanonicalArduinoRead write a byte to an Arduino, and then
   receives a serially transmitted string in response.

   The call/response Arduino sketch is here:
   https://gist.github.com/2980344

   Arduino sketch details at www.chrisheydrick.com
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <sys/ioctl.h>

#define DEBUG 1
  
int main(int argc, char *argv[])
{
  int fd, n, i;
  char buf[256];
  struct termios toptions;

  /* open serial port */
  fd = open("/dev/ttyTHS1", O_RDWR | O_NOCTTY);
  if(fd == -1) 
  {
    printf( "failed to open port\n" );
    return -1;
  }
  printf("fd opened as %d\n", fd);
  
  /* get current serial port settings */
  tcgetattr(fd, &toptions);
  /* set 9600 baud both ways */
  cfsetispeed(&toptions, B9600);
  cfsetospeed(&toptions, B9600);

  cfmakeraw(&toptions);
  
  /* 8 bits, no parity, one stop bit */
  //toptions.c_cflag &= ~PARENB;
  toptions.c_cflag |= (CLOCAL | CREAD);
  toptions.c_cflag &= ~CSTOPB;
  toptions.c_cflag &= ~CSIZE;
  toptions.c_cflag |= CS8;
  toptions.c_cflag &= ~CRTSCTS;
  /* Canonical mode */
  toptions.c_lflag |= ICANON;
  // Disable output processing
  toptions.c_oflag = 0;
  //toptions.c_oflag &= ~OPOST;
  /* commit the serial port settings */
  tcsetattr(fd, TCSANOW, &toptions);
  /* Send byte to trigger Arduino to send string back */
  //write(fd, "0", 1);
  /* Receive string from Arduino */
  // Flush anything already int eh serial buffer
  //tcflush(fd, TCIFLUSH);
  n = read(fd, buf, 256);
  /* insert terminating zero in the string */
  //buf[n] = 0;

  printf("%i bytes read, buffer contains: %s\n", n, buf);
 
  if(DEBUG)
    {
      printf("Printing individual characters in buf as integers...\n\n");
      for(i=0; i<n; i++)
	{
	  printf("Byte %5i:%5i:%5c\n",i+1, (int)buf[i], buf[i]+'0');
	}
      printf("\n");
    }

  return 0;
}

