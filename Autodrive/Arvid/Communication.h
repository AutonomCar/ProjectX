/*
* Arvid Englund
* 2017
*
* Communications classes and functions
* "Autonoma Elbilar"
*/


#include <string>       //String handling
#include <cstring>      // -||-
#include <stdio.h>      // standard input / output functions
#include <stdlib.h>
#include <sstream>


//Bluetooth:
#include <unistd.h>     // UNIX standard function definitions
#include <fcntl.h>      // File control definitions
#include <errno.h>      // Error number definitions
#include <termios.h>    // POSIX terminal control definitions



using namespace std;


class Command{
	//TODO
	//Used for imageprocessing to work correctly.
};



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



// CONTROLS

void startCAN(){ // Initialize CAN-bus
	system("sudo ip link set can0 up type can bitrate 500000");
}


// SPEED CONTROL

void speed(int dir){

    switch(dir) {
      case 1 : //forward
         system("cansend can0 140#01C8");
         break;
      case 0 : //back
         system("cansend can0 140#00C8");
         break;
      case 2 : //stop
         system("cansend can0 140#0200");
         break;
      default :
        cout << "Invalid Direction (setSpeed)" << endl;
   }

}

// TURNING

void turn(int dir){
    switch(dir) {
        case 1 : //left
            system("cansend can0 150#0100");
            break;
        case 2 : //right
            system("cansend can0 150#0000");
            break;
      case 0 : //straight
            system("cansend can0 150#025A");
            break;
      default :
        cout << "Invalid Direction (turn)" << endl;
   }
}


