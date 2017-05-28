/*
* Arvid Englund
* 2017
*
* inter-car communication
* Project: "Autonoma Elbilar"
* Halmstad University
*/


#include <string>       //String handling
#include <cstring>      // -||-
#include <stdio.h>      // standard input / output functions
#include <stdlib.h>		// needed for "system"
#include <sstream>		// stringstream
#include <fstream>		// file editing


using namespace std;

class car  {

public:
    int isCrossing;   //used for crossing
    char position;  // used for crossing
    int nextPos; // used in follow_car

    car(int a, char b, int c){
        isCrossing = a;
        position = b;
        nextPos = c;
    }
};



car getBT(car otherCar){
    //cout << "fetchInput()" << endl;

    char file_data[10];
    fstream read_file;
    read_file.open("/home/pi/Autodrive/AutodriveFilesArvids/TESTS/TEST3_Bluetooth/reciveBT", ios::in|ios::out); // open file

    if(!read_file.is_open()){
		cout << "File not opened..." << endl;
		return otherCar;
	}


    read_file >> file_data;           // read from the file
    cout << file_data << endl;

    read_file.close();

    otherCar.isCrossing = file_data[2];
    otherCar.isCrossing = otherCar.isCrossing - 48;
    cout << otherCar.isCrossing;
    otherCar.position =  file_data[3];
    cout << otherCar.position;
    otherCar.nextPos = file_data[4];
    otherCar.nextPos = otherCar.nextPos - 48;
    cout << otherCar.nextPos << endl;

    return otherCar;
}


int sendBT(car myCar){

    char file_data[10];
    fstream write_file;
    write_file.open("/home/pi/Autodrive/AutodriveFilesArvids/TESTS/TEST3_Bluetooth/sendBT", ios::in|ios::out); // open file

    if(!write_file.is_open()){
		cout << "File not opened..." << endl;
		return 1;
	}

	write_file << myCar.isCrossing;
	write_file << myCar.position;
	write_file << myCar.nextPos;

    file_data[99] = 'q';
    write_file.close();


    return 1;

}
