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
#include <stdlib.h>
#include <sstream>
#include <fstream>


using namespace std;

int fmu = 0; // front-middle ultrasound sensor
int fru = 0; // front-right ultrasound sensor
int sfu = 0; // side-front ultrasound sensor
int sbu = 0; // side-back ultrasound sensor
int lir = 0; // left ir sensor
int rir = 0; // right ir sensor

class Command{
	//TODO
	//Used for imageprocessing to work correctly.
};


// initialize the CAN system
void startCAN(){ // Initialize CAN-bus
	system("sudo ip link set can0 up type can bitrate 500000");
}


//Converting int to hex
template< typename T >
string int_to_hex(T i)
{
	stringstream stream;
	stream << ""
		//		<< std::setfill('0') << std::setw(sizeof(T) * 2)
		<< std::hex << i;

	return stream.str();
}

void sendCAN(int value, char action) { // value: the desired speed/angle (0-255), action: Forward, Backward, Stop, Angle
	string str;
	if (action == 'f') {// send speed
		str = "cansend can0 140#01" + int_to_hex(value);
	}
	if (action == 'b') {// send speed
		str = "cansend can0 140#00" + int_to_hex(value);
	}
	if (action == 's') {// STOP
		str = "cansend can0 140#0200";
	}
	if (action == 'a') {// send angle
		str = "cansend can0 150#02" + int_to_hex(value); // value max: 128?(0x80) min: 48(0x30) Limitation: the electric engine that steers
	}

    cout << action << endl;

	//cout << str.size() << endl;
	//cout << str << endl;
	char cmd[43];
	strcpy(cmd, str.c_str());

	//cout << cmd << endl;
	system(cmd);// Cmd linux command setSpeed action (forward,back,stop)

}


int hexCharToInt(char x){

    int y = x;
    if(y > 47 && y < 58)  //this covers 0-9
       y = y - 48;
    else if (y > 96 && y < 103) // this covers A-F
       y = y - 87;

    return y;
}

int processInput(int nn, char *data){ // n: the element to start iteration on, data: the array
    // update sensor data.
    cout << "processInput()" << endl;
    cout << data << endl;

    char a = data[0];
    string nr;
    string value1;
    string value2;
    string error;
    int n = nn;

    // iterate until relevant information is found, add to input data.
    while( (data[n] != 'x')&&(data[n] != 'q')&&(n<99) ){
        n++;
    }

    if(data[n] == 'q'){
        return 1;
    }

    n++;
    while(data[n] != '-'){ // first number
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
    while(data[n] != '-'){ // fourth number
        error = error + data[n];
        n++;
    }

    //cout << "conversion complete, nr: " << nr << ", value1: " << value1 << ", value2: " << value2 <<". " << endl;

    int x = atoi(nr.c_str());
    int y = atoi(value1.c_str());
    int z = atoi(value2.c_str());
    int err = atoi(error.c_str());

    int value = y + (z*256);

    if(err != 255){

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
                //cout << "bad switch number" << endl;
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
    //for(int i = 0; i < 10000; i++){} // give the python fetch program some time

    char data[100];
    fstream file;
    file.open("sensors", ios::in|ios::out); // open file
    file >> data;           // read from the file
    //cout << data << endl;

    //file <<""<< endl;       // empty the file

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




