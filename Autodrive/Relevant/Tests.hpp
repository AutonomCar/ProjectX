/*

Autonoma Elbilar

Testing functions


Arvid Englund
2017-06-04

*/


int a = 0;
int sleeping =  4000000;
int speed = 70;

int testIR(){
	
	a++;
    fetchInput();


	if(rirPassed>0){
		//cout << endl;
		cout << "R-IR Found - " << a << endl;
	}
	if(lirPassed>0){
		//cout << endl;
		cout << "L-IR Found - "<< a << endl;
   	}

	if( (lirPassed==1)&&(rirPassed==1) ){
		cout << endl;
		cout << "!! Line Found !! " << a << " --- Restarting test..." << endl;
		usleep(sleeping);
		cout << "Test Reset." << endl;
		return 1;
	}

}

int testIR2(){

	sendCAN(speed, 'f');
	while( !((lirPassed==1)&&(rirPassed==1)) ){
		fetchInput();
	}
	sendCAN(0, 's');
	cout << "Line found" << endl;
	usleep(sleeping);
	return 1;
}
