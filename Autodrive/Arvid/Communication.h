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
#include <fstream>



// Bluetooth
#include "Bluetooth.hpp"

// CAN-bus
#include "Can-bus.hpp"




using namespace std;


class Command{
	//TODO
	//Used for imageprocessing to work correctly.
};




// CONTROLS

void startCAN(){ // Initialize CAN-bus
	system("sudo ip link set can0 up type can bitrate 500000");
}


// SPEED CONTROL // TODO : panos did an implementation that mixed this verison with the previous, more flexible one.

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
            system("cansend can0 150#027a");
            break;
        case 2 : //right
            system("cansend can0 150#0230");
            break;
      case 0 : //straight
            system("cansend can0 150#0255");
            break;
      default :
        cout << "Invalid Direction (turn)" << endl;
   }
}

int hexCharToInt(char x){

    int y = x;
    if(y > 47 && y < 58)  //this covers 0-9
       y = y - 48;
    else if (y > 96 && y < 103) // this covers A-F
       y = y - 87;

    return y;
}


void updateSensors(){
    system("cansend can0 200#0000"); // initiate fetch data
    for(int i = 0; i < 10000; i++){} // give the python fetch program some time
    char data[100];
    fstream file;
    file.open("sensors", ios::in|ios::out); // open file
    file >> data;           // read from the file
    cout << data << endl;

    file <<""<< endl;       // empty the file

    file.close();


    // update sensor data.

    char a = data[0];
    int b = 0;
    int i = 0;

    while( (a != 'x')&&(a != '-') ){
        i++;
        a = data[i];
        cout << i << endl;
    }

    i++;
    b = hexCharToInt(data[i]);
    cout << "First: " << b << " - " << data[i] << endl;
    i++;
    b = hexCharToInt(data[i]);
    cout << "Second: " << b << " - " << data[i] << endl;

    a = data[i];
    while( (a != 'x')&&(a != '-') ){
        i++;
        a = data[i];
        cout << i << endl;
    }

    i++;
    b = hexCharToInt(data[i]);
    cout << "First: " << b << " - " << data[i] <<endl;
    i++;
    b = hexCharToInt(data[i]);
    cout << "Second: " << b << " - " << data[i] << endl;


    a = data[i];
    while( (a != 'x')&&(a != '-') ){
        i++;
        a = data[i];
        cout << i << endl;
    }

    i++;
    b = hexCharToInt(data[i]);
    cout << "First: " << b << " - " << data[i] << endl;
    i++;
    b = hexCharToInt(data[i]);
    cout << "Second: " << b << " - " << data[i] << endl;

}

