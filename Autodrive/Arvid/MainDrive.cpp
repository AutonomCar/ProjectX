
#include <string.h>
#include <iostream>
#include "Communication.h"
//#include "Navigation.h"



using namespace std;



int mode;
int FOLLOW_ROAD = 1; 	// Follow the road using ImageProcessing
int AVOID = 2;		// Pre coded route to avoid object op road
int FOLLOW_CAR = 3;	// Follow car at set distance.
int CROSSING = 4;		// Manage an intersection
int JUST_DRIVE = 5;	// Drive forward, turn when ecountering obstacles (roomba style)
int ERROR = 6;		// Something is wrong. Stop Car Immediatly.

int NORMAL = 1;
int BACK = 0;
int STOP = 2;

int LEFT = 1;
int RIGHT = 2;
int STRAIGHT = 0;



void updateMode(){

    /* // TESTING
	//Object within 100cm in front of car
	if( (getCAN(110, 1)>100) && (mode!=FOLLOW_CAR) && (mode == FOLLOW_ROAD) ) // Stop if object appears between the cars in FOLLOWING_CAR
		mode = AVOID;

	//Paralell line under car. When not in AVOID procedure.
	else if( (getCAN(110, 1)>100)  && getCAN(210, ) && (mode!=AVOID)) //TODO fix getCAN(IR FRONT)
		mode = CROSSING;

	//If there are lanes to follow
	else if(foundRightLine() || foundLeftLine()) //TODO
		mode = FOLLOWING_ROAD;


	//If there is no input to make a decision, drive until some input is aquired.
	else */ //TESTING
		mode = JUST_DRIVE;
}



void updateMode(int m){ // Force specific mode
	mode = m;
}



/* // TESTING
void avoid(){ //TODO check values and maybe fix for avoiding in a bend or turn in the road, HARDCODED :(
	interface.setSpeed(SLOW_SPEED);

	interface.setAngle(-0.5);
	int wait = 0;
	while(interface.getSonarFront()<100){	//wait until the object is not in front anymore.
		wait++;
		if(leftLineFound){
			interface.setSpeed(0);
			mode = ERROR;
		}
	}
	int return = wait;
	interface.setAngle(0.5);
	while(( wait>0) && (interface.getSonarFront()>100) ){ //straighten out
		wait--;
	}
	interface.setAngle(0);

	mode = FOLLOW_ROAD; 			// Use standard road following precedure until obstacle is passed.
	while(interface.getSonarSide()<30){
		ImageProcessor.run();		<<<<FIX
	}


	interface.setAngle(0.5);
	while(return>0){ 			//Turn until returned to original lane.
		return--;
		wait++;
	}
	interface.setAngle(-0.5);
	while( !foundRightLine || (wait>0) ){ 	//straighten out
		wait--;
	}
	interface.setAngle(0);

}

*/ // TESTING
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
    cout << "justDrive" << endl;
    usleep(1000000);

    cout << "Forward" << endl;
    speed(NORMAL);
    usleep(1000000);

    cout << "Stop" << endl;
    speed(STOP);
    usleep(1000000);

    cout << "Backward" << endl;
    speed(BACK);
    usleep(1000000);

    cout << "Stop" << endl;
    speed(STOP);
    usleep(1000000);

    cout << "Forward" << endl;
    speed(NORMAL);
    usleep(1000000);

    cout << "Turn Left" << endl;
    turn(LEFT);
    usleep(1000000);

    cout << "Turn Right" << endl;
    turn(RIGHT);
    usleep(1000000);

    cout << "Turn Straight" << endl;
    turn(STRAIGHT);
    usleep(1000000);

    cout << "Stop" << endl;
    speed(STOP);
    usleep(1000000);

}




void run(){

	switch(mode){
		/* // TESTING
		case 1:
			ImageProcessing.run();
		break;

		case 2:
			avoid();
		break;

		case 3:
			followCar();
		break;

		case 4:
			crossing();
		break;
        */ // TESTING
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

	while(1){
		updateMode();
		run();
	}

return 1;
}
