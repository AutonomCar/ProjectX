/*
* Arvid Englund
* 2017
*
* CAN-bus functions
* Project: "Autonoma Elbilar"
* Halmstad University
*/


#include <string>       //String handling
#include <cstring>      // -||-
#include <stdio.h>      // standard input / output functions
#include <stdlib.h>		// needed for "system"
#include <sstream>		// stringstream
#include <fstream>		// file editing


using namespace std;

int fmu = 110; 	// front-middle ultrasound sensor
int fru = 100; 	// front-right ultrasound sensor
int sfu = 100; 	// side-front ultrasound sensor
int sbu = 100; 	// side-back ultrasound sensor
int lir = 0; 	// left ir sensor
int rir = 0; 	// right ir sensor

class Command{
	//TODO
	//Used for imageprocessing to work correctly. ?
};

// TODO trim commennts and get rid of extra "cout"


void startCAN(){ // Initialize CAN-bus
	system("sudo ip link set can0 up type can bitrate 500000");
}


template< typename T >
string int_to_hex(T i)
{//Converting int to hex
	stringstream stream;
	stream << ""
		//		<< std::setfill('0') << std::setw(sizeof(T) * 2)
		<< std::hex << i;

	return stream.str();
}

void sendCAN(int value, char action) { // value: the desired speed/angle (0-255), action: Forward, Backward, Stop, Angle
	string str;
	if (action == 'f') {// send speed forward
		str = "cansend can0 140#01" + int_to_hex(value);
	}
	if (action == 'b') {// send speed backward
		str = "cansend can0 140#00" + int_to_hex(value);
	}
	if (action == 's') {// STOP
		str = "cansend can0 140#0200";
	}
	if (action == 'a') {// send angle
		str = "cansend can0 150#02" + int_to_hex(value); // value max: 128?(0x80) min: 48(0x30) Limitation: the electric engine that steers // TODO update numbers
	}

	cout << action << endl;

    char cmd[43]; // set size, just make sure it is large enough for the entire commandline
	strcpy(cmd, str.c_str());

	//cout << cmd << endl;
	system(cmd);
}

/*// TODO remove, not needed anymore ?
int hexCharToInt(char x){ //convert a hex-char to an int variable

    int y = x;
    if(y > 47 && y < 58)  //this covers 0-9
       y = y - 48;
    else if (y > 96 && y < 103) // this covers A-F
       y = y - 87;

    return y;
}
*/

int processInput(int nn, char *data){ // n: the element to start iteration on, data: the array from file
    // update sensor data.
    cout << "processInput()";
    cout << data << endl;

    char a = data[0];
    string nr;
    string value1;
    string value2;
    string error;
    int n = nn;

    // iterate until relevant information is loaded, add to input data.

    while( (data[n] != 'x')&&(data[n] != 'q')&&(n<99) ){
        n++;
    }

    if(data[n] == 'q'){
        return 1;
    }

    n++;
    while(data[n] != '-'){ // first number (id)
        nr = nr + data[n];
        n++;
    }

    n++;
    while(data[n] != '-'){  // second number
        value1 = value1 + data[n];
        n++;
    }

    n++;
    while(data[n] != '-'){ // third number
        value2 = value2 + data[n];
        n++;
    }

    n++;
    while(data[n] != '-'){ // fourth number (if number is negative(255) its a bad value)
        error = error + data[n];
        n++;
    }

    //cout << "conversion complete, nr: " << nr << ", value1: " << value1 << ", value2: " << value2 <<". " << endl;

    int x = atoi(nr.c_str());
    int y = atoi(value1.c_str());
    int z = atoi(value2.c_str());
    int err = atoi(error.c_str());

    int value = y + (z*256);

    if(err != 255){ // if number is negative its a bad value, disregard it
    	//TODO update this to use the negative values for acceleration etc

        switch(x){
            case 101:
                fmu = value; // update front sensor data
                //cout << "fmu set." << endl;
                break;
            case 102:
                fru = value; // update front-right sensor data
                //cout << "fru set." << endl;
                break;
            case 103:
                sfu = value; // update side-front front sensor data
                //cout << "sfu set." << endl;
                break;
            case 104:
                sbu = value; // update side-back sensor data
                //cout << "sbu set." << endl;
                break;
            case 105:
                lir = value; // update left ir sensor data
                //cout << "sbu set." << endl;
                break;
            case 106:
                rir = value; // update right sensor data
                //cout << "sbu set." << endl;
                break;
            default:
                cout << "(sensors) bad id number" << endl;
                break;
        }
    }

    if(n<99){
        processInput(n, data);
    }

    return 1;

}

int fetchInput(){
    //cout << "fetchInput()" << endl;
    system("cansend can0 200#0000"); // initiate fetch data
    //for(int i = 0; i < 10000; i++){} // give the python fetch program some time?

    char data[100];
    fstream file;
    file.open("sensors", ios::in|ios::out); // open file
    file >> data;           // read from the file
    //cout << data << endl;

    //file <<""<< endl;       // empty the file, not needed? Python resets the data everytime

    file.close();


    processInput(0, data); // uses the data to update the sensor values

    //cout << endl;
    //cout << "Updated values: " << endl;

    //cout << "fmu: " << fmu << endl;
    //cout << "fru: " << fru << endl;
    //cout << "sfu: " << sfu << endl;
    //cout << "sbu: " << sbu << endl;

    return 1;
}




