/*
* Arvid Englund
* 2017
*
* Communications classes and functions
* "Autonoma Elbilar"
*/


#include <string>

//Bluetooth:
#include <stdio.h>      // standard input / output functions
#include <stdlib.h>
#include <string.h>     // string function definitions
#include <unistd.h>     // UNIX standard function definitions
#include <fcntl.h>      // File control definitions
#include <errno.h>      // Error number definitions
#include <termios.h>    // POSIX terminal control definitions


using namespace std;


class Command{
	//TODO
	//Used for imageprocessing to work correctly.
};

class Sensors {
	//TODO
};



void startCAN(){ // Initialize CAN-bus
	system("sudo ip link set can0 up type can bitrate 500000");
}

void sendCAN(int id, char msg1, int msg2){ // Send message to CAN-bus. msg1: more precise id. msg2:transmitted value

	string str = "cansend can0" + id + "#0"+ msg1 + int_to_hex(msg2);
	cout << "str size: " << str.size() << endl;

	cout << str << endl;
	//int strS = str.size()+1;
	char cmd[43]; //preferably size of str +1: strS, that does not work for some reason however

	strcpy(cmd, str.c_str());


	system(cmd);// Cmd linux command setSpeed 100


};

void requestUpdate(Sensors a) { //Needs to request an update first (it takes some time to go through)
	//TODO
}


void update(Sensors a) { //update the values of Sensors
	
	int str[] = { 1,2,3,4,5,6 }; //get the msg //TODO

	int adress;

	adress += str[0] * 100;
	adress += str[1] * 10;
	adress += str[2] * 1;

	switch (adress) {//TODO

		case 111:
			//update the sensor value
			break; //optional

		case 112:
			//update sensor
			break; //optional

		default: //Optional
			//return fail?
	}
}


template< typename T >
std::string int_to_hex(T i) // Converting to a string of hex.
{
	std::stringstream stream;
	stream << ""
//	<< std::setfill('0') << std::setw(sizeof(T) * 2)
	<< std::hex << i;
	return stream.str();
}



void connectBL(){

	// Open Port:

	int USB = open( "/dev/ttyUSB0", O_RDWR| O_NOCTTY );



	// Set Parameters:

	struct termios tty;
	struct termios tty_old;
	memset (&tty, 0, sizeof tty);

	/* Error Handling */
	if ( tcgetattr ( USB, &tty ) != 0 ) {
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
	tcflush( USB, TCIFLUSH );
	if ( tcsetattr ( USB, TCSANOW, &tty ) != 0) {
	   std::cout << "Error " << errno << " from tcsetattr" << std::endl;
	}
}

void writeBL(string msg){ // Send Message via bluetooth
	unsigned char cmd[] = "INIT \r" + msg; // added +msg
	int n_written = 0,
	    spot = 0;

	do {
	    n_written = write( USB, &cmd[spot], 1 );
	    spot += n_written;
	} while (cmd[spot-1] != '\r' && n_written > 0);
}

string readBL(){
	int n = 0,
	    spot = 0;
	char buf = '\0';

	/* Whole response*/
	char response[1024];
	memset(response, '\0', sizeof response);

	do {
	    n = read( USB, &buf, 1 );
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

