
#include <string.h>
#include <iostream>
#include "Bluetooth.hpp"


using namespace std;


int main(){
	char msg[10] = "msg:A";
	string response;
	int x;
	x = 0;

    setSlave();
    //setMaster("adressen in");

	int USB = connectBT();

	while(1){

		cout << x << endl;

        writeBT(USB, msg);

        response = readBT(USB);


        cout << response << "!" << endl;

		x++;
	}

ſreturn 0;
}
