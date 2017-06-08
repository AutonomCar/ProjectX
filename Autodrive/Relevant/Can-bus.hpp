/*
* Arvid Englund
* 2017
*
* CAN-bus
* Project: "Autonoma Elbilar"
* Halmstad University
*/

#include <string.h>     // string library
#include <iostream>     // in/out functions (usleep)
#include <stdio.h>      // standard input / output functions
#include <stdlib.h>     //
#include <unistd.h>     // UNIX standard function definitions

#include <cstring>      // -||-
#include <sstream>		// stringstream
#include <fstream>		// file editing



using namespace std;

int fmu = 200; 	// front-middle ultrasound sensor
int fru = 200; 	// front-right ultrasound sensor
int sfu = 100; 	// side-front ultrasound sensor
int sbu = 100; 	// side-back ultrasound sensor
int lirPassed = 0; 	// left ir sensor
int rirPassed = 0; 	// right ir sensor
int irPassed = 0;


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
		str = "cansend can0 150#02" + int_to_hex(value);
	}

    char cmd[43]; // set size, just make sure it is large enough for the entire commandline
	strcpy(cmd, str.c_str());

	system(cmd);
}


int processInput(int nn, char *file_data){ // n: the element to start iteration on, data: the array from file
    // update sensor data.
	// iterate until relevant information is loaded, add to input data.

    char a = file_data[0];
    string nr;
    string value1;
    string value2;
    string error;
    int n = nn;

    

    while( (file_data[n] != 'x')&&(file_data[n] != 'q')&&(n<99) ){
        n++;
    }

    if(file_data[n] == 'q'){
        return 1;
    }

    n++;
    while(file_data[n] != '-'){ // first number (id)
        nr = nr + file_data[n];
        n++;
    }

    n++;
    while(file_data[n] != '-'){  // second number
        value1 = value1 + file_data[n];
        n++;
    }

    n++;
    while(file_data[n] != '-'){ // third number
        value2 = value2 + file_data[n];
        n++;
    }

    n++;
    while(file_data[n] != '-'){ // fourth number (if number is negative(255) its a bad value)
        error = error + file_data[n];
        n++;
    }



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
		lirPassed = value;
                break;

            case 104: 
		rirPassed = value;
                break;

            case 105:
                irPassed = value;
                break;

            case 106:
                sbu = value;
                break;

            case 107:
                sfu = value;
                break;

            default:
                cout << "(sensors) bad id number: " << endl;
                break;
        }
    }
    else{
        //cout << "--- discarded: " << x << ":" << value << endl;
    }

    if(n<99){
        processInput(n, file_data);
    }
    return 1;
}

int fetchInput(){

    char file_data[100];
    fstream read_file;
    read_file.open("/home/pi/Autodrive/testingAutodrive/sensors", ios::in|ios::out); // open file

    if(!read_file.is_open()){
		cout << "File not opened..." << endl;
		return 1;
	}

    read_file >> file_data;           // read from the file

    file_data[99] = 'q';
    read_file.close();

    processInput(0, file_data); // uses the data to update the sensor values


    return 1;
}



//Debugging printing:

    //cout << endl;
    //cout << "Updated values: " << endl;

    //cout << "fmu: " << fmu << endl;
    //cout << "fru: " << fru << endl;
    //cout << "sfu: " << sfu << endl;
    //cout << "sbu: " << sbu << endl;


