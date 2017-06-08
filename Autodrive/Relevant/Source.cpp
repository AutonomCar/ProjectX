/*

Autonoma Elbilar

Main file.



Arvid Englund
2017-06-04



*/


// files
#include "Can-bus.hpp"  // communication by CAN-bus
#include "camera.hpp"   // camera functions
#include "navigation.hpp" // navigation functions
#include "bluetooth.hpp" // inter-car communication
#include "Tests.hpp"	// Test functions


// utility
#include <string.h>     // string library
#include <iostream>     // in/out functions (usleep)
#include <stdio.h>      // standard input / output functions
#include <stdlib.h>     //
#include <unistd.h>     // UNIX standard function definitions

using namespace cv;
using namespace std;


int STANDARD = 1; // Standard mode
int FOLLOW_ROAD = 1; // Follow road using the camera
int FOLLOW_CAR = 2; // Follow the other car
int CROSSING = 3; // Handle an intersection
int AVOID = 4; // Avoid obstacle
int IDLE = 5; // Start Mode, Wait for input (wave a hand in front of sensor)
int FREE_MODE = 6; // Drive without road, avoid obstacles
int STOP = 7;

int mode = STANDARD; // The cars current situation and response

 int shortsleep = 1000000;    // sleep
 int longsleep =  4000000;

 int standard_speed = 70;
 int slow_speed = 40;

 int counter = 10; // Purpose: slow down the amount of file reading while car is using camera, a simple counter to lagging performance
                  // used in "followRoad()"

 int fmuClose = 10; // Set obstacle sensitivity for front-middle sensor
 int fmu_emer = 20; // Set obstacle sensitivity for front-middle sensor (emergency brake)


// Depends on car(steering controls). Black car = 89, 55, 120. White car: 100, 85, 125
 int straight_wheels = 100;
 int max_right_angle = 85;
 int max_left_angle = 125;




navigation navObj = navigation(5,1); // updated via navigation.hpp


car other_car = car(0,'a',9); // updated via bluetooth.hpp
car my_car = car(0,'a',9); // Update before start.


void emerstop(){ // Emergency stop
	
	if(fmu<fmu_emer){
		sendCAN(0, 's');
		cout << "Emergency stop" << endl;
	}

	while(fmu<fmu_emer){
		fetchInput();
	}
}

int updateMode(){

    if( (fmu<fmuClose)&&(mode!=FOLLOW_CAR) ){ // check for obstacles
        cout << "AVOID" << endl;
        return AVOID;
    }


    if( (lirPassed==1)&&(rirPassed==1) ){ // check for crossing
        cout << "CROSSING" << endl;
        return CROSSING;

    }


    cout << "STANDARD" << endl;
    return STANDARD; // if no other issues, set to standard mode

}

void updateMode(int m){ // set specific mode
	mode = m;
}


void followRoad(){

    sendCAN(standard_speed, 'f');

    for(int i=0; i<counter; i++){
    processingCamera();
    }

}


void followCar(){
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


void turn(){ // DIFFERENT FROM BLACK CAR

    int dir = decision2(navObj);
	if(dir == 0){dir = 1;} // TEST

    switch(dir){
	    case 1:
	        //turn right
	        cout << "right turn" << endl;
	        sendCAN(max_right_angle, 'a');
		usleep(7200000); // let the car turn
	        break;

	    case 2:
	        //turn left
	        cout << "left turn" << endl;
	        sendCAN(max_left_angle, 'a');
		usleep(6800000);
	        break;

        	case 4:
	        //double right turn
	        cout << "double right turn" << endl;
	        sendCAN(max_right_angle, 'a');
		usleep(14300000);
	        break;

	    case 0:
	        // go straight
	        cout << "going straight" << endl;
	        sendCAN(standard_speed, 'f');
		usleep(4000000);
	        break;

	    default:
	        cout << "error turn!" << endl;
	        break;
    }


}

void crossing(){
    navObj = driving2(navObj); // update navigation data
    // update my info
    my_car.isCrossing = 1;
    my_car.position = getCPos();
    my_car.nextPos = navObj.next;

    sendBT(my_car); // update other car's information of me
    other_car = getBT(other_car); // update my information about other car

    if(other_car.isCrossing == 1){ // check if the other car is in an intersection
        if(other_car.position == my_car.position){  // check if other car is in the same intersection
            sendCAN(00, 's');
            while(other_car.isCrossing == 1) {  // stop until other car leaves intersection
                other_car = getBT(other_car);
            }
            sendCAN(standard_speed, 'f');
        }
    }

    turn(); // perform the turn

    my_car.isCrossing = 0; // set crossing to false
    //sendBT(my_car); // update other car's information of me

}



int avoid(){ // avoiding obstacle on road
	cout << "--- avoid" << endl;
	int x = 0;

/*
// Simple version (Hardcoded)
    sendCAN(max_left_angle, 'a'); // turn left
    cout << "turning left" << endl;
    usleep(4000000);

    while(fru<70){
        fetchInput();
	emerstop();
        for(int i = 0; i<counter; i++){
            processingCamera();
        }
        cout << "1 while" << endl;
    }

    while(sfu>65){
        fetchInput();
	emerstop();
        for(int i = 0; i<counter; i++){
            processingCamera();
        }
        cout << "2 while" << endl;
    }

	while(sfu<70){
        fetchInput();
	emerstop();
        for(int i = 0; i<counter; i++){
            processingCamera();
        }
        cout << "3 while" << endl;---

    }

    while(sbu<70){
        fetchInput();
	emerstop();
        for(int i = 0; i<counter; i++){
            processingCamera();
        }
        cout << "4 while" << endl;
    }


    sendCAN(max_right_angle, 'a'); // turn left
    cout << "turning right" << endl;
    usleep(4500000);


*/

// Complex version
    sendCAN(max_left_angle, 'a'); // turn left
    cout << "turning left" << endl;
    usleep(shortsleep); // to make sure it is not detecting the line of the current road

    while(lirPassed != 1){ // continue until left sensor passes first line
        fetchInput();
	emerstop();
    }
    cout << "left IR found!" << endl;

    cout << "straighten" << endl;
    sendCAN( (max_right_angle) , 'a');


    // follow the road until the obstacle has been passed:
    cout << "follow road";
    while( sbu>40 ){
        fetchInput();
	emerstop();
        for(int i = 0; i<counter; i++){
            processingCamera();
        }
    }

    while(sbu<40){
        fetchInput();
	emerstop();
        for(int i = 0; i<counter; i++){
            processingCamera();
        }
    }
    cout << "-- DONE" << endl;

    // return to original lane
    sendCAN(max_right_angle, 'a'); // turn right
    cout << "turn right" << endl;

    while(lirPassed != 1){ // continue until right sensor passes first line
        fetchInput();
	  emerstop();
    }
    cout << "IR found!" << endl;

     fetchInput();
	emerstop();

    sendCAN(max_left_angle, 'a'); // turn left to straighten out again
    usleep(shortsleep);

    cout << "avoid -- done" << endl;

    return 1;
}


void idle(){ // starting mode.

    cout << "waiting for go signal" << endl;
    sendCAN(00,'s');

    while(fmu>10){ // wait until fmu detects a "go signal"
        fetchInput();
    }

	cout << "---" << endl;
    while(fmu<10){ // wait until fmu detects a "go signal"
        fetchInput();
    }
    cout << "==GO==" << endl;

}


int freeMode(){ // just a simple roomba-esque driving feature (not used)

	sendCAN(standard_speed, 'f');

	if(fmu<100){
		sendCAN(55, 'a'); // Turn Right to avoid obstacle
        if( (fmu<50) || (fru<30) ){ // if too close
            sendCAN(00, 's'); // stop
            usleep(shortsleep); // wait
            sendCAN(89, 'a'); // set steering forward
            sendCAN(slow_speed, 'b'); // reverse
            while( (fmu<50) || (fru<30) ){} // while too close, reverse
            sendCAN(00, 's'); // stop
        }
    }

	else{
		sendCAN(89, 'a');	// Turn Forward
	}


}






void run(){

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
			break;

		default:
			mode = STANDARD;
	}

	fetchInput(); // update sensor data
	emerstop();
	mode = updateMode(); // update the mode

}

int main(){

    // Startup sequence
    cout << "--- START                  ***"<<endl;
    startCAN();
    cout << "--- CAN started            ***" << endl;
    initCamera();
    findLanes();
    initNav(navObj, 2, 1); // Update depending on start position
    system("cansend can0 200#0000"); // initiate fetch data
    cout << "--- First, send for data   ***" << endl;
    cout << "--- STARTUP IS DONE...IDLE ***" << endl;
    usleep(shortsleep); // wait 0.5 sec.

	updateMode(5);// wait for start signal (wave a hand in front of front-middle sensor)
	while(1){
		run();
		//testIR();
	}

return 1;
}
