#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "sensordata.hpp"
#include "autodrive.hpp"
#include "crossingPoint.hpp"
#include "lastNextObj.hpp"


#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>

#define _AUTODRIVE_SHOWCANNY
#define _AUTODRIVE_SHOWHOUGH

#undef _DEBUG
#include "../imageprocessor/imageprocessor.hpp"



using namespace cv;
using namespace std;

/*Random funktionen*/
int randomize(int min, int max) {

	int rNum = 0;
	srand(time(NULL));//Behövs för att kunna få äkta random värden hela tiden
	rNum = rand() % (max - min + 1) + min;

	return rNum;

}

lastNextObj driving(lastNextObj car) {

	/*if (car.last == 0)
	{
	//Gör något
	}
	else {*/

	lastNextObj *carPtr;
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
			return *carPtr;
		}
		else if (r == 2) {
			*next = 8; //The road the car will drive too
			return *carPtr;
		}
		else if (r == 3) {
			*next = 7; //The road the car will drive too
			return *carPtr;
		}
	}//End of last=1
	else if (*last == 2) {
		r = randomize(1, 3);
		if (r == 1) {
			*next = 9; //The road the car will drive too
			return *carPtr;
		}
		else if (r == 2) {
			*next = 5; //The road the car will drive too
			return *carPtr;
		}
		else if (r == 3) {
			*next = 7; //The road the car will drive too
			return *carPtr;
		}

	}//end of last = 2
	else if (*last == 3) {
		r = randomize(1, 3);
		if (r == 1) {
			*next = 5; //The road the car will drive too
			return *carPtr;
		}
		else if (r == 2) {
			*next = 7; //The road the car will drive too
			return *carPtr;
		}
		else if (r == 3) {
			*next = 8; //The road the car will drive too
			return *carPtr;
		}

	}//End of last = 3
	else if (*last == 4) {
		r = randomize(1, 3);
		if (r == 1) {
			*next = 5; //The road the car will drive too
			return *carPtr;
		}
		else if (r == 2) {
			*next = 9; //The road the car will drive too
			return *carPtr;
		}
		else if (r == 3) {
			*next = 8; //The road the car will drive too
			return *carPtr;
		}

	}//End of last = 4
	else if (*last == 5) {
		r = randomize(1, 2);
		if (r == 1) {
			*next = 4; //The road the car will drive too
			return *carPtr;
		}
		else if (r == 2) {
			*next = 10; //The road the car will drive too
			return *carPtr;
		}

	}//End of last = 5
	else if (*last == 6) {
		r = randomize(1, 2);
		if (r == 1) {
			*next = 4; //The road the car will drive too
			return *carPtr;
		}
		else if (r == 2) {
			*next = 1; //The road the car will drive too
			return *carPtr;
		}

	}//End of last = 6
	else if (*last == 7) {
		r = randomize(1, 2);
		if (r == 1) {
			*next = 1; //The road the car will drive too
			return *carPtr;
		}
		else if (r == 2) {
			*next = 10; //The road the car will drive too
			return *carPtr;
		}

	}//End of last = 7
	else if (*last == 8) {
		r = randomize(1, 2);
		if (r == 1) {
			*next = 3; //The road the car will drive too
			return *carPtr;
		}
		else if (r == 2) {
			*next = 6; //The road the car will drive too
			return *carPtr;
		}

	}//End of last = 8
	else if (*last == 9) {
		r = randomize(1, 2);
		if (r == 1) {
			*next = 2; //The road the car will drive too
			return *carPtr;
		}
		else if (r == 2) {
			*next = 6; //The road the car will drive too
			return *carPtr;
		}

	}
	else if (*last == 10) {
		r = randomize(1, 2);
		if (r == 1) {
			*next = 3; //The road the car will drive too
			return *carPtr;
		}
		else if (r == 2) {
			*next = 2; //The road the car will drive too
			return *carPtr;
		}
	}//End of last = 10
	else {
		return *carPtr;
	}
	/*}*/
}

int startAndProcessImg() 
{
	string filename = "testdrive.mp4";
	int counter = 0; // Testar uppdatera vägvalen efter 100 counts
	int loopCounter = 100; // Testet kollar detta, Uppdateras med 100 varje gång 
	/*TEST*/
	lastNextObj car = lastNextObj(1, 1);
	lastNextObj *objPtr;
	objPtr = &car;
	int * objPtrLast;
	int * objPtrNext;
	objPtrLast = &car.last;
	objPtrNext = &car.next;
	/*END TEST*/
	VideoCapture capture(filename);// 0 for the camera
	Mat frame;
	if (!capture.isOpened())
		throw "Error, could not capture any frame from the camera.";
	string window = "w";
	namedWindow(window, 1);
	
	while (!Autodrive::imageProcessor::init_processing(&frame)) { // while cant find any frame? try again?
		capture >> frame;
	}

	for (;;) // Basicly the same as while(true)
	{
		capture >> frame;
		if (frame.empty()) {
			capture.open(filename);
			continue;
		}

		/*Prints out the angle between -1 and 1 (left and right)*/
		std::cout << Autodrive::imageProcessor::continue_processing(frame).angle << endl;
		
		/*imgProc = Autodrive::imageProcessor::continue_processing(frame);
		imgProc2.setAngle(imgProc.angle);

		if (imgProc.angle == imgProc2.angle) 
		{
			
			if (imgProc2.angle < -0.4 ) 
			{
				imgProc2.setSpeed(-1.0);
			}
			 if (imgProc2.angle  > 0.4 )
			{
				imgProc2.setSpeed(1.0);
			}
			else 
			{ 
				imgProc2.setSpeed(0.4); 
			}

			//std::cout << "angle: "<< imgProc2.angle << " speed: "<<imgProc2.speed << endl;
		}
		*/
		

		/*Displaying the frame on a window*/
		Autodrive::show_image(frame, 1, window);
		waitKey(10); // waits to display frame
		counter++;
		if (counter == loopCounter)
		{
			*objPtr = driving(*objPtr);
			objPtrLast = &objPtr->last; //Updated "Where it is comming from"
			objPtrNext = &objPtr->next; //Updated "Where it is going"
			std::cout << "THIS IS Last: " << *objPtrLast << endl;
			std::cout << "THIS IS Next: " << *objPtrNext << endl;
			loopCounter = loopCounter + 100;
		}
	}

	return 0;
}

 

int main()
{
	std::cout << "main" << endl;
	
	startAndProcessImg();
	
	/*
	crossingPoint a = crossingPoint(1, 2, 3, 4);
	crossingPoint b = crossingPoint(5, 6, 7, 0);
	crossingPoint c = crossingPoint(8, 0, 9, 10);
	*/
	/*
	lastNextObj car = lastNextObj(1,1);
	lastNextObj *objPtr;
	objPtr = &car;
	int * objPtrLast;
	int * objPtrNext;
	objPtrLast = &car.last;
	objPtrNext = &car.next;
	*/

	while (1) {
		/*
		*objPtr = driving(*objPtr);
		objPtrLast = &objPtr->last;
		objPtrNext = &objPtr->next;

		std::cout << "Last: " << *objPtrLast << endl;
		std::cout << "Next: " << *objPtrNext << endl;
		//cin.get();// wait for key press
		*/
	}
	
	return 0;
}
