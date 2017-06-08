/*

Panos Tioukalias
2017

Navigation

Autonoma Elbilar


*/

char currentPos = 'x';



class navigation {//navigation object
public:
	int last;
	int next;

	navigation(int lastPlace, int nextPlace) {
		last = lastPlace;
		next = nextPlace;


	}

};


void initNav(navigation obj, int x, int y){
    obj.last = x;
    obj.next = y;
}

/*
int getLast() {
		return last;
}

int getNext() {
		return next;
}

int setLast(int a) {
		last = a;
		return last;
}
int setNext(int b) {
		next = b;
		return next;
}
*/


char getCPos(){
    return currentPos;
}



/*Left turn*/
void turnLeft() {

	std::cout << "Turn left" << endl;

}
/*Right turn*/
void turnRight() {

	std::cout << "Turn right" << endl;

}

/*Move forward, keep moving*/
void moveStraight() {

	std::cout << "Move straight" << endl;

}

/*Random funktionen*/
int randomize(int min, int max) {

	int rNum = 0;
	srand(time(NULL));//Behövs för att kunna få äkta random värden hela tiden
	rNum = rand() % (max - min + 1) + min;

	return rNum;

}

navigation driving(navigation car) {

	/*if (car.last == 0)
	{
	//Gör något
	}
	else {*/

	navigation *carPtr;
	carPtr = &car;
	int *last;
	int *next;
	last = &carPtr->last;
	next = &carPtr->next;
	int r = 0;

	*last = *next;

	if (*last == 1) { //The road the car is comming from
		r = randomize(1, 3);
		if (r == 1) {
			*next = 9; //The road the car will drive too
			currentPos = 'a';
			return *carPtr;
		}
		else if (r == 2) {
			*next = 8; //The road the car will drive too
			currentPos = 'a';
			return *carPtr;
		}
		else if (r == 3) {
			*next = 7; //The road the car will drive too
			currentPos = 'a';
			return *carPtr;
		}
	}//End of last=1
	else if (*last == 2) {
		r = randomize(1, 3);
		if (r == 1) {
			*next = 9; //The road the car will drive too
			currentPos = 'a';
			return *carPtr;
		}
		else if (r == 2) {
			*next = 5; //The road the car will drive too
			currentPos = 'a';
			return *carPtr;
		}
		else if (r == 3) {
			*next = 7; //The road the car will drive too
			currentPos = 'a';
			return *carPtr;
		}

	}//end of last = 2
	else if (*last == 3) {
		r = randomize(1, 3);
		if (r == 1) {
			*next = 5; //The road the car will drive too
			currentPos = 'a';
			return *carPtr;
		}
		else if (r == 2) {
			*next = 7; //The road the car will drive too
			currentPos = 'a';
			return *carPtr;
		}
		else if (r == 3) {
			*next = 8; //The road the car will drive too
			currentPos = 'a';
			return *carPtr;
		}

	}//End of last = 3
	else if (*last == 4) {
		r = randomize(1, 3);
		if (r == 1) {
			*next = 5; //The road the car will drive too
			currentPos = 'a';
			return *carPtr;
		}
		else if (r == 2) {
			*next = 9; //The road the car will drive too
			currentPos = 'a';
			return *carPtr;
		}
		else if (r == 3) {
			*next = 8; //The road the car will drive too
			currentPos = 'a';
			return *carPtr;
		}

	}//End of last = 4
	else if (*last == 5) {
		r = randomize(1, 2);
		if (r == 1) {
			*next = 4; //The road the car will drive too
			currentPos = 'b';
			return *carPtr;
		}
		else if (r == 2) {
			*next = 10; //The road the car will drive too
			currentPos = 'b';
			return *carPtr;
		}

	}//End of last = 5
	else if (*last == 6) {
		r = randomize(1, 2);
		if (r == 1) {
			*next = 4; //The road the car will drive too
			currentPos = 'b';
			return *carPtr;
		}
		else if (r == 2) {
			*next = 1; //The road the car will drive too
			currentPos = 'b';
			return *carPtr;
		}

	}//End of last = 6
	else if (*last == 7) {
		r = randomize(1, 2);
		if (r == 1) {
			*next = 1; //The road the car will drive too
			currentPos = 'b';
			return *carPtr;
		}
		else if (r == 2) {
			*next = 10; //The road the car will drive too
			currentPos = 'b';
			return *carPtr;
		}

	}//End of last = 7
	else if (*last == 8) {
		r = randomize(1, 2);
		if (r == 1) {
			*next = 3; //The road the car will drive too
			currentPos = 'c';
			return *carPtr;
		}
		else if (r == 2) {
			*next = 6; //The road the car will drive too
			currentPos = 'c';
			return *carPtr;
		}

	}//End of last = 8
	else if (*last == 9) {
		r = randomize(1, 2);
		if (r == 1) {
			*next = 2; //The road the car will drive too
			currentPos = 'c';
			return *carPtr;
		}
		else if (r == 2) {
			*next = 6; //The road the car will drive too
			currentPos = 'c';
			return *carPtr;
		}

	}
	else if (*last == 10) {
		r = randomize(1, 2);
		if (r == 1) {
			*next = 3; //The road the car will drive too
			currentPos = 'c';
			return *carPtr;
		}
		else if (r == 2) {
			*next = 2; //The road the car will drive too
			currentPos = 'c';
			return *carPtr;
		}
	}//End of last = 10
	else {
        currentPos = 'x';
		return *carPtr;
	}
	/*}*/
}

navigation driving2(navigation carNav) { // Second Map


	navigation *carPtr;
	carPtr = &carNav;
	int *last;
	int *next;
	last = &carPtr->last;
	next = &carPtr->next;
	int r = 0;

	*last = *next;

	if (*last == 1) { //The road the car is comming from
        *next = 2; //The road the car will drive too
        currentPos = 'a';
        return *carPtr;
	}//End of last=1

	else if (*last == 2) {
        *next = 3; //The road the car will drive too
        currentPos = 'b';
        return *carPtr;
	}//end of last = 2

	else if (*last == 3) {
		r = randomize(1, 2);
		if (r == 1) {
			*next = 1; //The road the car will drive too
			currentPos = 'c';
			return *carPtr;
		}
		else if (r == 2) {
			*next = 4; //The road the car will drive too
			currentPos = 'c';
			return *carPtr;
		}
	}//End of last = 3

	else if (*last == 4) {
        *next = 5; //The road the car will drive too
        currentPos = 'd';
		return *carPtr;
	}//End of last = 4

	else if (*last == 5) {
		r = randomize(1, 2);
		if (r == 1) {
			*next = 2; //The road the car will drive too
			currentPos = 'a';
			return *carPtr;
		}
		else if (r == 2) {
			*next = 6; //The road the car will drive too
			currentPos = 'a';
			return *carPtr;
		}
	}//End of last = 5


	else if (*last == 6) {
	        *next = 4; //The road the car will drive too
        	currentPos = 'c';
		return *carPtr;
	}//End of last = 6

	else {
        currentPos = 'x';
		return *carPtr;
	}
	/*}*/
}

int decision(navigation navObj) {

	navigation *carPtr;
	carPtr = &navObj;
	int *last;
	int *next;
	last = &carPtr->last;
	next = &carPtr->next;

	/*Alla vilkor för vänstersväng*/
	if (*last == 1 && *next == 7 ||
		*last == 2 && *next == 9 ||
		*last == 3 && *next == 5 ||
		*last == 4 && *next == 8 ||
		*last == 6 && *next == 4 ||
		*last == 7 && *next == 1 ||
		*last == 8 && *next == 6 ||
		*last == 9 && *next == 2 )
	{
		return 1;
	}
	/*Alla vilkor för högersväng*/
	else if (*last == 1 && *next == 9 ||
			*last == 2 && *next == 7 ||
			*last == 3 && *next == 8 ||
			*last == 4 && *next == 5 ||
			*last == 5 && *next == 4 ||
			*last == 7 && *next == 10 ||
			*last == 8 && *next == 3 ||
			*last == 10 && *next == 2)
	{
		return 2;
	}
	/*Alla vilkor för körning rakt fram*/
	else if(*last == 1 && *next == 8 ||
		*last == 2 && *next == 5 ||
		*last == 3 && *next == 7 ||
		*last == 4 && *next == 9 ||
		*last == 5 && *next == 10 ||
		*last == 6 && *next == 1 ||
		*last == 9 && *next == 6 ||
		*last == 10 && *next == 3)
	{
		return 0;
	}


}

int decision2(navigation navObj) {

	navigation *carPtr;
	carPtr = &navObj;
	int *last;
	int *next;
	last = &carPtr->last;
	next = &carPtr->next;

	/*Alla vilkor för vanlig högersväng*/
	if ( (*last == 1 && *next == 2) ||
	     (*last == 3 && *next == 1)||
	     (*last == 5 && *next == 6)||
	     (*last == 6 && *next == 4) )
	{
		return 1;
	}
	/*Alla vilkor för dubbel högersväng*/
	else if ( (*last == 2 && *next == 3) ||
		  (*last == 4 && *next == 5) )
	{
		return 4;
	}

	/*Alla vilkor för vanlig köra rakt*/
	if ( (*last == 5 && *next == 2) ||
	     (*last == 3 && *next == 4) )
	{
		return 0;
	}

}


