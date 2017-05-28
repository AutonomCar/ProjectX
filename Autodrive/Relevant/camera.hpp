/*
Panos Tioukalias
2017

Camera
Autonoma Elbilar

//Hemsidan //TODO
*/

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "autodrive.hpp"
//#include "lastNextObj.hpp"
#include "navigation.hpp"

#include <stdio.h>      // standard input / output functions
#include <stdlib.h>
#include <string.h>     // string function definitions
/*#include <unistd.h>     // UNIX standard function definitions
#include <fcntl.h>      // File control definitions
#include <errno.h>      // Error number definitions
#include <termios.h>    // POSIX terminal control definitions
*/


#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>

#define _AUTODRIVE_SHOWCANNY
#define _AUTODRIVE_SHOWHOUGH

#undef _DEBUG
#include "../imageprocessor/imageprocessor.hpp"



string filename = "testdrive.mp4";
VideoCapture capture(0);// 0 for the camera
							   /*Change the width and height*/
Mat frame;
Mat * framePtr;
string window = "w";
string *theWindow;
int newAngle;
int lastNewAngle;


using namespace cv;
using namespace std;



/*Change the angle from between (-1) and 1 to 70-120

80 middle
55 max left
120 max right
*/
int convAngle(double angle) {

	if (angle == 0) { // För att styra rakt


		if(lastNewAngle > 99) // Om det senaste styrvärdet var över 89
		{
			newAngle = 120;
			lastNewAngle = newAngle; // uppdatera lastNewAngle
			return newAngle;
		}
		else if (lastNewAngle < 79) // Om det senaste styrvärdet var under 89
		{
			newAngle = 55;
			lastNewAngle = newAngle; // uppdatera lastNewAngle
			return newAngle;
		}
		else{
            newAngle = 89;
			lastNewAngle = newAngle; // uppdatera lastNewAngle
			return newAngle;

		}
	}
	else if ((angle > -1) && (angle < 0)) //för sväng åt vänster
	{
		if ((angle <-0.8)) //Om svängen är skarp
		{
			newAngle = 120;
			lastNewAngle = newAngle;
			return newAngle;
		}
		else
		{//Vanligt
			newAngle = (89 - (31.0 * angle));
		}
	}
	else if ((1.01 > angle) && (angle > 0))//för sväng åt höger
	{
		if (angle > 0.8)
		{ //Om svängen är skarp
			newAngle = 55;
			lastNewAngle = newAngle;
			return newAngle;
		}
		else
		{ //Vanligt
			newAngle = (89 - (34 * angle));
		}
	}
	//cout << "newAngle: " << newAngle << endl;

	lastNewAngle = newAngle;
	return newAngle;

}
/*
template< typename T >
std::string int_to_hex(T i)
{
	std::stringstream stream;
	stream << ""
		//		<< std::setfill('0') << std::setw(sizeof(T) * 2)
		<< std::hex << i;

	return stream.str();
}
*/

int initCamera() {

	system("sudo modprobe bcm2835-v4l2");


	capture.set(CV_CAP_PROP_FRAME_WIDTH, 120);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, 90);

	framePtr = &frame;
	theWindow = &window;
	namedWindow(window, 1);
	if (!capture.isOpened())
		throw "Error, could not capture any frame from the camera.";


    return 1;

}

int findLanes(){

    cout << "findLanes" << endl;

    while(!Autodrive::imageProcessor::init_processing(framePtr)){
    capture >> *framePtr;
	Autodrive::show_image(*framePtr, 1, *theWindow);
    waitKey(10); // waits to display frame
    }
    /*
	if(Autodrive::imageProcessor::init_processing(framePtr)){
        return 1;
	}
    */
	return 1;
}

int processingCamera()
{
    //cout << "processingCamera" << endl;
	capture >> *framePtr;
	//if (&frame.empty()) {
	//	capture.open(filename);
	//continue;
	//}

	double cmdAngle = Autodrive::imageProcessor::continue_processing(*framePtr).angle;

    Autodrive::show_image(*framePtr, 1, *theWindow);
    waitKey(10); // waits to display frame

	int intToSend = convAngle(cmdAngle);
	//cout << "NewAngle " << intToSend << endl;

	if( (intToSend < 75)||(intToSend > 100) )  {
        sendCAN(50, 'f');
	}

	sendCAN(intToSend, 'a');
	intToSend = 0;

	return 1;
}
