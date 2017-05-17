/*
* Arvid Englund
* 2017
*
* Bluetooth functions
* "Autonoma Elbilar"
*/

#include <stdio.h>      // standard input / output functions
#include <stdlib.h>     //
#include <unistd.h>     // UNIX standard function definitions
#include <fcntl.h>      // File control definitions
#include <errno.h>      // Error number definitions
#include <termios.h>    // POSIX terminal control definitions

#include <string>       //String handling
#include <cstring>      // -||-


using namespace std;


void setSlave(){
    system("sudo sdptool add --channel=1 SP");

    system("sudo mknod -m 666 /dev/rfcomm0 c 216 0");

    system("sudo rfcomm watch /dev/rfcomm0 1 /sbin/agetty rfcomm0 115200 linux");

}


void setMaster(){ // adress; XX:XX:XX:XX:XX

    // B8:28:EB:C4:3E:8C

    system("sudo rfcomm connect 0 ");
}

int connectBT(){



	// Open Port:

	int bluetooth = open( "/dev/rfcomm0", O_RDWR| O_NOCTTY );



	// Set Parameters:

	struct termios tty;
	struct termios tty_old;
	memset (&tty, 0, sizeof tty);

	/* Error Handling */
	if ( tcgetattr ( bluetooth, &tty ) != 0 ) {
	   std::cout << "Error " << errno << " from tcgetattr: " << strerror(errno) << std::endl;
	}

	/* Save old tty parameters */
	tty_old = tty;

	/* Set Baud Rate */
	cfsetospeed (&tty, (speed_t)B9600);
	cfsetispeed (&tty, (speed_t)B9600);

	/* Setting other Port Stuff */
	tty.c_cflag     &=  ~PARENB;            // Make 8n1
	tty.c_cflag     &=  ~CSTOPB;
	tty.c_cflag     &=  ~CSIZE;
	tty.c_cflag     |=  CS8;

	tty.c_cflag     &=  ~CRTSCTS;           // no flow control
	tty.c_cc[VMIN]   =  1;                  // read doesn't block
	tty.c_cc[VTIME]  =  5;                  // 0.5 seconds read timeout
	tty.c_cflag     |=  CREAD | CLOCAL;     // turn on READ & ignore ctrl lines

	/* Make raw */
	cfmakeraw(&tty);

	/* Flush Port, then applies attributes */
	tcflush( bluetooth, TCIFLUSH );
	if ( tcsetattr ( bluetooth, TCSANOW, &tty ) != 0) {
	   std::cout << "Error " << errno << " from tcsetattr" << std::endl;
	}

	return bluetooth; // Return bluetooth to be able to send it to writeBL and readBL
}

void writeBT(int bluetooth, char msg[1000]){
	char cmd[7] = "INIT \r";
	int n_written = 0,
	    spot = 0;

	do {
	    n_written = write( bluetooth, &cmd[spot], 1 );
	    spot += n_written;
	} while (cmd[spot-1] != '\r' && n_written > 0);
}

string readBT(int bluetooth){
	int n = 0,
	    spot = 0;
	char buf = '\0';

	/* Whole response*/
	char response[1024];
	memset(response, '\0', sizeof response);

	do {
	    n = read( bluetooth, &buf, 1 );
	    sprintf( &response[spot], "%c", buf );
	    spot += n;
	} while( buf != '\r' && n > 0);

	if (n < 0) {
	    std::cout << "Error reading: " << strerror(errno) << std::endl;
	}
	else if (n == 0) {
	    std::cout << "Read nothing!" << std::endl;
	}
	else {
	    std::cout << "Response: " << response << std::endl;
	}


	return response; //Added return data
}


