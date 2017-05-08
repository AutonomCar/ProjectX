#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "sensordata.hpp"
#include "autodrive.hpp"
#include "crossingPoint.hpp"
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



using namespace cv;
using namespace std;


/*Change the angle from between (-1) and 1 to 70-120*/
int convAngle(double angle) {
	int newAngle = 0;

	if (angle == 0) {
		newAngle = 90;
	}
	else if (angle < 0) {
		//vänster
		newAngle = ((10.0 * angle) + 80);
	}
	else if (1.01 > angle > 0) {
		//Höger
		newAngle = ((30 * angle) + 90);
	}
	return newAngle;

}

	int startAndProcessImg()
	{
		//string filename = "testdrive.mp4";
		//int counter = 0; // Testar uppdatera vägvalen efter 100 counts
		//int loopCounter = 100; // Testet kollar detta, Uppdateras med 100 varje gång 
		/*TEST*/
		double cmdAngle = 0;
		int changingAngle;
		navigation car = navigation(1, 1);
		navigation *objPtr;
		objPtr = &car;
		int * objPtrLast;
		int * objPtrNext;
		objPtrLast = &car.last;
		objPtrNext = &car.next;
		/*END TEST*/
		VideoCapture capture(0);// 0 for the camera
		capture.set(CV_CAP_PROP_FRAME_WIDTH, 120);
		capture.set(CV_CAP_PROP_FRAME_HEIGHT, 90);
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
				capture.open(0);
				continue;
			}

			/*Prints out the angle between -1 and 1 (left and right)*/
			//std::cout << Autodrive::imageProcessor::continue_processing(frame).angle << endl;
			cmdAngle = Autodrive::imageProcessor::continue_processing(frame).angle;
			convAngle(cmdAngle);
			
			//convAngle(cmdAngle);
			//std::cout << convAngle(cmdAngle) << endl;
	

			//sendCAN(150,'1', int msg2)

			//Autodrive::imageProcessor::continue_processing(frame);
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
		/*	counter++;
			if (counter == loopCounter)
			{
				*objPtr = driving(*objPtr);
				decision(*objPtr);
				//objPtrLast = &objPtr->last; //Updated "Where it is comming from"
				//objPtrNext = &objPtr->next; //Updated "Where it is going"
				std::cout << "THIS IS Last: " << *objPtrLast << endl;
				std::cout << "THIS IS Next: " << *objPtrNext << endl;
				loopCounter = loopCounter + 100;
			}
			*/
		}

		return 0;
	}

	/*Converting int to hex*/
	template< typename T >
	std::string int_to_hex(T i)
	{
		std::stringstream stream;
		stream << ""
			//		<< std::setfill('0') << std::setw(sizeof(T) * 2)
			<< std::hex << i;
		return stream.str();
	}

	void driveGo(int a, char action) {
		String str;
		if (action == 'f') {
			str = "cansend can0 140#01" + int_to_hex(action);
		}
		if (action == 'b') {
			str = "cansend can0 140#00" + int_to_hex(action);
		}
		if (action == 's') {
			str = "cansend can0 140#02" + int_to_hex(action);
		}

		cout << str.size() << endl;
		cout << str << endl;
		char cmd[43];
		strcpy(cmd, str.c_str());

		system(cmd);// Cmd linux command setSpeed action (forward,back,stop)

	}

	void startCAN() {
		system("sudo ip link set can0 up type can bitrate 500000");

	}

	void sendCAN(int idint, char msg1, int msg2) { // Send message to CAN-bus. msg1: more precise id. msg2:transmitted value

		char aa = '1';
		int b = (idint % 100);
		int c = (b % 10);
		char bb = (char)(b - c);
		char cc = (char)c;
		char idchar[] = { aa, bb, cc };
		string idstr = idchar;
		string str = "cansend can0" + idstr + "#0" + msg1 + int_to_hex(msg2);
		cout << "str size: " << str.size() << endl;

		cout << str << endl;
		//int strS = str.size()+1;
		char cmd[43]; //preferably size of str +1: strS, (it does not work for some reason)

		strcpy(cmd, str.c_str());


		system(cmd);// Cmd linux command setSpeed 100


	};



	/*void sendCAN() {

		int fd;
		canmsg_t frame;
		*/
		/* open(),
		bit rate comes from parameters at proc/sys/dev/Can */
		/*
			fd = open("/dev/can0", O_RDWR);

			frame.id = 100;
			frame.length = 2;
			frame.data[0] = 5;
			frame.data[1] = 0;
			write(fd, &frame, 1); // ! count is number of frames, not byte

			read(fd, &frame, 1);  // ! count is number of frames, not byte
			printf("received CAN Frame Id %ld, DLC=%d\n", frame.id, frame.length);
			return 0;

		}
		*/

		/*
		void justDrive(int x)
		{
		}
		*/

int main()
{
	std::cout << "main" << endl;
	//startCAN();
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
		
	/*	driveGo(100,'f');
		for (int i = 0; i < 2000; i++) {}
		driveGo(0,'s'); //check if 0x00 is a problem (instead if ex 0x0164)
		for (int i = 0; i < 2000; i++) {}
		driveGo(100,'b');
		for (int i = 0; i < 2000; i++) {}
		driveGo(0,'s');
		for (int i = 0; i < 2000; i++) {}*/
		

		/* //Köra driving()
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
