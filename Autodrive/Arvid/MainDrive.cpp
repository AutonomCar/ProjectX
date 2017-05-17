
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
	// TODO set imageprocessor start PANOS

	//If there is no input to make a decision, drive until some input is aquired.
	else{
        if(mode != JUST_DRIVE){
            cout << "Just Driving" << endl;
        }
		mode = JUST_DRIVE;
	}

}

void updateMode(int m){ // Force specific mode (for debugging purposes)
	mode = m;
}

void avoid(){ //TODO check values and maybe fix for avoiding in a bend or turn in the road
	// need imageprocessing methods for this. it is a mix. PANOS
	sendCAN(00,'s');

    /*
    -Slow down
    -turn left until passed the center line.
    -follow road until obstacle is passed
    -turn right until passed the center line.
    -update mode.
    */
}

void followCar(){ // PANOS
	if(fmu>120){
		sendCAN(120, 'f'); // TODO check number for fastest forward speed
		// Follow road
	}
	else if(fmu<80){         // Slow down if close
		sendCAN(50, 'f');
		// Follow road
	}
	else if(fmu<30){
		sendCAN(00, 's');	// Stop if too close
	}
	else{                    // Otherwise Normal Speed
        sendCAN(100, 'f');
		// Follow road
	}
//Problems:
//1. Setting the mode.
//2. Avoiding objects. The car in front have to tell the car in the back? Presume no obstacles...?
}

void crossing(){ // TODO HILLBERG
    //check other cars pos with BT
	if(){

	}
	else{
		// execute turn or go forward and then follow road again
	}
}

void justDrive(){
	sendCAN(100, 'f');

	if(50<fmu<100){
		sendCAN(48, 'a'); // Turn Right //TODO double check numbers
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
		sendCAN(80, 'a');	// turn left //TODO double check numbers
		sendCAN(100, 'b');	// Backwards
		usleep(500000); 	// 1/2 sec pause
		sendCAN(00, 's'; 	// Stop
	}
	else{
		sendCAN(55, 'a');	// Turn Forward //TODO double check numbers
	}

}

void run(){

	fetchInput(); // update sensor data
	updateMode(); // Decide action, depending on the sensor data

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

		case 6:
			sendCAN(00,'s'); // Something wrong. Stop Car
		break;

		default:
			cout << "No mode" << endl;
			mode = ERROR;
	}

}

int main(){
    cout << "START"<<endl;
    startCAN();
    cout << "CAN started" << endl;
    fetchInput(); // get an initial input value (there might be a delay)
    usleep(10000000); // wait 1 sec.

    while(1){ // Driving around
        run();
    }

return 1;
}
