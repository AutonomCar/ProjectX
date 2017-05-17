
#include <string.h>
#include <iostream>
#include "Bluetooth.hpp"
#include "Can-bus.hpp"



using namespace std;



int mode;
int FOLLOW_ROAD = 1;	// Follow the road using ImageProcessing
int AVOID = 2;			// Pre coded route to avoid object on road
int FOLLOW_CAR = 3;		// Follow car at set distance.
int CROSSING = 4;		// Manage an intersection
int JUST_DRIVE = 5;		// Drive forward, turn when ecountering obstacles (roomba style)
int ERROR = 6;			// Something is wrong. Stop Car Immediatly.


void updateMode(){


	//Object within 100cm in front of car
	if( (fmu>100) && (mode!=FOLLOW_CAR) && (mode == FOLLOW_ROAD) ){ // Stop if object appears between the cars in FOLLOWING_CAR
        if(mode != AVOID){
            cout << "Avoiding obstacle" << endl;
        }
		mode = AVOID;
	}

	//Paralell line under car. When not in AVOID procedure.
	else if( (lir==1) && (rir == 1) && (mode!=AVOID) ){
		if(mode != CROSSING){
            cout << "In Intersection" << endl;
		}
		mode = CROSSING;
	}
	//If there are lanes to follow
	// TODO set imageprocessor start


	//If there is no input to make a decision, drive until some input is aquired.
	else{
        if(mode != JUST_DRIVE){
            cout << "Just Driving" << endl;
        }
		mode = JUST_DRIVE;
	}

}



void updateMode(int m){ // Force specific mode
	mode = m;
}


void avoid(){ //TODO check values and maybe fix for avoiding in a bend or turn in the road, HARDCODED :(
	sendCAN(00,'s');

    /*
    Slow down
    turn left until passed the center line.
    follow road until obstacle is passed
    turn right until passed the center line.
    update mode.
    */
}
/* // TESTING
void followCar(){
	if(getCAN(110,1)>110)
        // Accelerate if to far
		sendCAN(ADRESS!, ADRESS!, FAST_SPEED); // TODO
	else if(getCAN(110,1)<90)                   // Slow down if to close
		sendCAN(ADRESS!, ADRESS!, SLOW_SPEED); // TODO
    else                                        // Otherwise Normal Speed
        sendCAN(ADRESS!, ADRESS!, NORMAL_SPEED);
//Problems:
//1. Setting up the scenario where one car can follow another
//2. Avoiding objects. The car in front have to tell the car in the back?
}


void crossing(){
    //check other cars pos with BT
	if(){

	}
	else
		mode = JUST_DRIVE;

}
*/ // TESTING

void justDrive(){
	sendCAN(100, 'f');

	if(50<fmu<100){
		sendCAN(48, 'a'); // Turn Right //TODO
	}
	else if(fmu<30){
		sendCAN(00, 's'); 	// Stop
		usleep(100000); 	// 1/10 sec pause
		sendCAN(100, 'b');	// Backwards
		usleep(500000); 	// 1/2 sec pause
		sendCAN(00, 's'), 	// Stop
	}
	else if (fru<30){
		sendCAN(00, 's'); 	// Stop
		usleep(100000); 	// 1/10 sec paus
		sendCAN(80, 'a');	// turn left //TODO
		sendCAN(100, 'b');	// Backwards
		usleep(500000); 	// 1/2 sec pause
		sendCAN(00, 's'; 	// Stop
	}
	else{
		sendCAN(55, 'a');	// Turn Forward //TODO
	}

}




void run(){

	switch(mode){
		case 1:
			//ImageProcessing.run();
		break;

		case 2:
			avoid();
		break;

		case 3:
			//followCar();
		break;

		case 4:
			//crossing();
		break;

		case 5:
			justDrive();
		break;

		default:
			cout << "No mode" << endl;
	}

}


int main(){
    cout << "START"<<endl;
    startCAN();
    cout << "CAN started" << endl;

    while(1){ // Driving around
        fetchInput();
		updateMode();
		run();
	    //usleep(1000000); // TEMP

    }

return 1;
}
