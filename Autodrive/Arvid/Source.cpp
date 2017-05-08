
#include <string.h>
#include <iostream>
#include "Communication.h"


using namespace std;


int main(){
	char msg[10] = "msg:A";
	string response;
	int x;
	x = 0;

	int USB = connectBL();

	while(1){

		cout << x << endl;

        writeBL(USB, msg);

        response = readBL(USB);


        cout << response << "!" << endl;

		x++;
	}

return 0;
}
