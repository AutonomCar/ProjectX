/*
* Arvid Englund
* 2017
*
* Communications classes and functions
* "Autonoma Elbilar"
*/

#include <string>


using namespace std;



class Sensors {
	//TODO
};



void startCAN(){
	system("sudo ip link set can0 up type can bitrate 500000");
}

void sendCAN(int id, char msg1, int msg2){

	string str = "cansend can0" + id + "#0"+ msg1 + int_to_hex(msg2);
	cout << "str size: " << str.size() << endl;

	cout << str << endl;
	//int strS = str.size()+1;
	char cmd[43]; //preferably size of str +1: strS

	strcpy(cmd, str.c_str());


	system(cmd);// Cmd linux command setSpeed 100


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


template< typename T >
std::string int_to_hex(T i)
{
	std::stringstream stream;
	stream << ""
//	<< std::setfill('0') << std::setw(sizeof(T) * 2)
	<< std::hex << i;
	return stream.str();
}

