/*

Arvid Englund

Halmstad
2017-05-20

project: Autonoma Elbilar

** open source **

*/



#include "Can-bus.hpp"  // communication by CAN-bus
#include "camera.hpp"   // camera functions
//#include "navigation.hpp" // navigation functions
//#include "bluetooth.hpp" // inter-car communication

#include <string.h>     // string library
#include <iostream>     // in/out functions (usleep)
#include <stdio.h>      // standard input / output functions
#include <stdlib.h>     //
#include <unistd.h>     // UNIX standard function definitions

//using namespace cv;
using namespace std;

int mode;                   // The cars current situation and response
int STANDARD = 1;           // standard mode (change depending on demonstration scenario: FOLLOW_ROAD, FOLLOW_CAR, FREE_MODE)
int FOLLOW_ROAD = 1;	    // Follow road using the camera
int FOLLOW_CAR = 2;			// Follow the other car
int CROSSING = 3;	        // Handle a intersection
int AVOID = 4;		        // Avoid obstacle
int IDLE = 5;               // Wait for input
int FREE_MODE = 6;		    // Drive without road, avoid obstacles
int STOP = 7;
int shortsleep = 00000;    // sleep ONLY USE SLEEP FOR TESTING

int standard_speed = 50;
int slow_speed = 40;

int counter = 10;           // Purpose: slow down the amount of file reading while car is using camera, a simple counter to lagging performance

int test = 1;

int fmuClose = 100; // Set obstacle sensitivity
int fruClose = 30;

int straight_wheels = 110;
int max_right_angle = 80;
int max_left_angle = 130;


/*
navigation navObj = navigation(1,5); // updated via navigation.hpp

car other_car = car(0,'a',9); // updated via bluetooth in carcommunicaion.hpp
car my_car = car(0,'a',9); // Update before start.
*/

int updateMode(){

//    cout << "--- update mode" << endl;

    fetchInput(); // update sensor data


    if( (fmu<fmuClose)&&(mode!=FOLLOW_CAR) ){ // check for obstacles
        if(fmu<30){
            cout << "STOP" << endl;
            return STOP;
        }

        else{
        cout << "AVOID" << endl;
        return AVOID;
        }
    }

	int a = lir() + rir();

    if( (a == 2)&& (mode != CROSSING) ){ // check for crossing
        cout << "CROSSING" << endl;
        //navObj = driving(navObj); // update navigation data // FIX
        sendCAN(slow_speed, 'f');
        return CROSSING;

    }
    else if( (a == 2) && (mode == CROSSING) ){
            cout << "leaving intersection.." << endl;
    }

    cout << "STANDARD" << endl;
    return STANDARD; // if no other issues, set to standard mode

}

void updateMode(int m){ // Force specific mode (for debugging purposes)
	mode = m;
}


void followRoad(){

  //  cout << "--- followRoad" << endl;
   // usleep(shortsleep); // wait 0.5 sec. // TESTING

    sendCAN(standard_speed, 'f');

    for(int i=0; i<counter; i++){
    processingCamera();
    }

}


void followCar(){
    cout << "--- followCar" << endl;


    if(fmu<90){
        sendCAN( (standard_speed-10), 'f');
    }
    else if(fmu>110){
        sendCAN( (standard_speed+10), 'f');
    }

    else if(fmu<50){
        sendCAN( 00, 's');
    }

    else{
        sendCAN(standard_speed, 'f');
    }




}
// manuveure
void turn(){
    cout << "--- turn" << endl;
    /*int dir = decision(navObj);

    switch(dir){

    case 1:
        //turn left
        cout << "left turn" << endl;
        break;

    case 2:
        //turn right
        cout << "right turn" << endl;
        break;

    case 0:
        // go straight
        cout << "going straight" << endl;
        break;

    default:
        cout << "error turn!" << endl;
        break;

    }

    */// TODO find the exact values to turn well. test it out.

}

void crossing(){ // simple version
    cout << "--- crossing" << endl;
	sendCAN(00, 's');
	while(1){	}
  /*  cout << "nav last: " << navObj.last << "nav next: " << navObj.next << endl;
    // get. info from other car....


    if(other_car.isCrossing == 1){
        if(other_car.position == currentPos){
            sendCAN(00, 's');
            while(other_car.isCrossing == 1) {}
        }
        turn();
    }
    else{
        turn();
    }

cout << currentPos << endl;
usleep(3000000); // TESTING
*/

}

int avoid(){ // avoiding obstacle on road
	cout << "--- avoid" << endl;
	sendCAN(00, 's');
	sendCAN(slow_speed, 'f'); // set slower speed for this.


    sendCAN(max_left_angle, 'a'); // turn left
    cout << "turning left" << endl;
    while(lir() != 1){ // until left sensors passes first line
        fetchInput();
        //usleep(shortsleep); // TESTING
        //cout << "while rir!=1" << endl;
    }

    fetchInput();

    // TEST!
    cout << "straighten" << endl;
    sendCAN(max_right_angle, 'a');
    usleep(shortsleep);

    fetchInput();

    cout << "first IR found" << endl;
    while(lir() != 1){ // until left sensors passes second line
        fetchInput();
        //usleep(shortsleep); // TESTING
        //cout << "while rir!=1" << endl;
    }
    cout << "second IR found" << endl;

    // Straighten out
    cout << "straighten" << endl;
    sendCAN(max_right_angle, 'a');
    usleep(shortsleep);


    while(sfu<50){
        fetchInput();
        processingCamera();
    }

    while(sbu<50){
        fetchInput();
        processingCamera();
    }


    // return to original lane
    sendCAN(55, 'a'); // turn right
    cout << "turn right" << endl;
    while(rir() != 1){ // until right sensors passes first line
        fetchInput();
        //usleep(shortsleep); // TESTING
        //cout << "while rir!=1" << endl;
    }
    fetchInput();
    while(rir() != 1){ // until right sensors passes second line
        fetchInput();
        //usleep(shortsleep); // TESTING
        //cout << "while rir!=1" << endl;
    }

    sendCAN(120, 'a'); // turn left


    return 1;
}


void idle(){
    cout << "--- idling" << endl;
sendCAN(00,'s');
    while(fmu>10){ // wait until fmu detects a "go signal"
        fetchInput();
    }
    updateMode();

}


int freeMode(){
    cout << "--- justDrive" << endl;


	sendCAN(standard_speed, 'f');

	if(50<fmu<100){
		sendCAN(55, 'a'); // Turn Right to avoid obstacle

        while( (fmu<50) || (fru<30) ){ // if too close, reverse
            sendCAN(89, 'a'); // set steering forward
            sendCAN(slow_speed, 'b'); // reverse
            updateMode();
        }
    }

	else{
		sendCAN(89, 'a');	// Turn Forward
	}


}






void run(){
    //cout << endl;
   // cout << "*run*" << endl;
    switch(mode){
		case 1:
            followRoad();
		break;

		case 2:
			followCar();
		break;

		case 3:
			crossing();
		break;

		case 4:
			avoid();
		break;

		case 5:
			idle();
		break;

		case 6:
			freeMode();
		break;

		case 7:
            sendCAN(00,'s'); // STOP

		default:
			mode = STANDARD;
	}

	mode = updateMode();
	//cout << mode << endl;
}

int main(){
    cout << "--- START                  ***"<<endl;
    startCAN();
    cout << "--- CAN started            ***" << endl;
    initCamera();
    findLanes();
    //initNav(navObj, 1, 5); // Change depending on start posittion.(se img: Intersection)
    system("cansend can0 200#0000"); // initiate fetch data //TESTING
    cout << "--- First, send for data   ***" << endl;


    updateMode(STANDARD); // set starting mode

    //sendCAN(standard_speed, 'f');


    cout << "--- STARTUP IS DONE: LOOP  ***" << endl;

    usleep(shortsleep); // wait 0.5 sec.

    while(1){
      run();
	//cout << "Ir: " << lirPassed << rirPassed << irPassed << endl;

    }
return 1;
}
