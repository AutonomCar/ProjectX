//Communications classes and functions

#include <string>


using namespace std;


class Command{

};


class Sensors {

};


void requestUpdate(Sensors a) { //Needs to request an update first (it takes some time to go through)

}
void update(Sensors a) { //update the values of Sensors
	int str[] = { 1,2,3,4,5,6 }; //get the msg

	int adress;

	adress += str[0] * 100;
	adress += str[1] * 10;
	adress += str[2] * 1;

	switch (adress) {
		case 111:
			
			break; //optional
		case constant - expression:
			statement(s);
			break; //optional

				   // you can have any number of case statements.
		default: //Optional
			statement(s);
	}
}

