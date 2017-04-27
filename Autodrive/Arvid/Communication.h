/*
* Arvid Englund
* 2017
*
* Communications classes and functions
* "Autonoma Elbilar"
*/

#include <string>


using namespace std;


class Command{
	//TODO
};


class Sensors {
	//TODO
};


void requestUpdate(Sensors a) { //Needs to request an update first (it takes some time to go through)
	//TODO
}


void update(Sensors a) { //update the values of Sensors
	
	int str[] = { 1,2,3,4,5,6 }; //get the msg //TODO

	int adress;

	adress += str[0] * 100;
	adress += str[1] * 10;
	adress += str[2] * 1;

	switch (adress) {//TODO

		case 111:
			//update the sensor value
			break; //optional

		case 112:
			//update sensor
			break; //optional

		default: //Optional
			//return fail?
	}
}

