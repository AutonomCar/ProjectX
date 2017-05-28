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
int lastLir = 0;
int rirPassed = 0; 	// right ir sensor
int lastRir = 0;
int irPassed = 0;
int lastIR = 0;

int carSpeed = 0;
int carAngle = 0;



void startCAN(){ // Initialize CAN-bus
	system("sudo ip link set can0 up type can bitrate 500000");
}


int ir(){
    if(irPassed == 1){
        irPassed = 0;
        return 1;
    }
    else
        return 0;
}

int lir(){
    if(lirPassed == 1){
        lirPassed = 0;
        return 1;
    }
    else
        return 0;
}

int rir(){
    if(rirPassed == 1){
        rirPassed = 0;
        return 1;
    }
    else
        return 0;
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

	//cout << action << endl;

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

int processInput(int nn, char *file_data){ // n: the element to start iteration on, data: the array from file
    // update sensor data.
    //cout << "processInput: ";
    //cout << file_data << endl;

    char a = file_data[0];
    string nr;
    string value1;
    string value2;
    string error;
    int n = nn;

    // iterate until relevant information is loaded, add to input data.

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

            case 103: // make sure that the value is 1 only on "rising edge"
                if(lastLir == 0){
                    lirPassed = value; // left IR
                    lastLir = value;
                    //cout << "lir set." << endl;
                }
                if(value == 0){
                        lirPassed = 0;
                        lastLir = 0;
                }
                break;

            case 104:
                if(lastRir == 0){
                    rirPassed = value; // left IR
                    lastRir = value;
                    //cout << "rir set." << endl;
                }
                if(value == 0){
                        rirPassed = 0;
                        lastRir = 0;
                }

                break;

            case 105:
                if(lastIR == 0){
                    irPassed = value; // left IR
                    lastIR= value;
                    //cout << "ir set." << endl;
                }
                if(value == 0){
                        irPassed = 0;
                        lastIR = 0;
                }
                //cout << "sbu set." << endl;
		break;

            case 106:
                sfu = value;
                break;

            case 107:
                sbu = value;
                break;

            default:
                cout << "(sensors) bad id number: " << endl;
                break;
        }
    }
    else{
        //cout << "--- discarded: " << x << ":" << value << endl;
    }

    //cout <<x<<":-:"<<value<<endl;

    if(n<99){
        processInput(n, file_data);
    }

    return 1;
}

int fetchInput(){
    //cout << "fetchInput()" << endl;
    //system("cansend can0 200#0000"); // initiate fetch data // CHECK

    char file_data[100];
    fstream read_file;
    read_file.open("/home/pi/Autodrive/testingAutodrive/sensors", ios::in|ios::out); // open file

    if(!read_file.is_open()){
		cout << "File not opened..." << endl;
		return 1;
	}


    read_file >> file_data;           // read from the file
    //cout << data << endl;

    file_data[99] = 'q';
    read_file.close();

    //cout << "fetchedInput: " << file_fetch_data_lol << endl;
    processInput(0, file_data); // uses the data to update the sensor values

    //cout << endl;
    //cout << "Updated values: " << endl;

    //cout << "fmu: " << fmu << endl;
    //cout << "fru: " << fru << endl;
    //cout << "sfu: " << sfu << endl;
    //cout << "sbu: " << sbu << endl;

    return 1;
}



