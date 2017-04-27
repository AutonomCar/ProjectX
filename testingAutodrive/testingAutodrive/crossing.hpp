#include "autodrive.hpp"
#include "sensordata.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include "../imageprocessor/imageprocessor.hpp"

/*
K�r tills irDownLeft/Right uppt�cker att en rak linje ------


*/
using namespace cv;

namespace Autodrive {
	
	int usFront;
	int usFrontRight;
	int irDownRight;
	int irDownLeft;

	int order = 0;

	bool crossing = false; // Aktivera n�r vi vill k�ra i korsning
	bool crossingLineEnter = false; // linjen precis innan korsningen (Global)
	
	/*Variabler f�r v�gar bilen kan ta i en korsning*/
	int leftTurn = 0;
	int rightTurn = 0;
	int moveStraight = 0;

	command run(command lastCommand, Mat *Mat)
	{
		usFront = SensorData::ultrasound.front;
		usFrontRight = SensorData::ultrasound.frontRight; // �ndra i sensordata
		irDownLeft = SensorData::infrared.frontLeft;
		irDownRight = SensorData::infrared.frontRight;
		//irActive = 0;

		lastCommand.setSpeed(0.4);//�ndra farten(om det beh�vs)

		/*Om ir sensorerna under bilen uppt�cker att vi k�r �ver korsningsaktiverande
		linje*/
		if (irDownLeft == 1 || irDownRight == 1) 
		{
			//irActive = 1;
			crossing = true;
			if (!crossingLineEnter) 
			{
				crossingLineEnter = true;// gl�m inte att �ndra till false n�r man k�rt ut fr�n korsning
				order = 1;

			}

			/*
			if (!irActive)
			{
				irActive = true;
				crossing = true;
			}
			if (irActive)
			{
			irActive = false;
			crossing = false; // kanske? eller crossing = false n�r korsningscenariot �r slut?
			}

			*/
		}

		if (crossing) 
		{
			if (crossingLineEnter && order == 1) 
			{
				

				if (leftTurn == 1)
				{
					lastCommand.setAngle(-1);


				}
				if (rightTurn == 1)
				{
					lastCommand.setAngle(1);

				}
				if (moveStraight == 1)
				{
					while (1) //&& andra bilen inte �r i crossing mode
					{
						/*N�gon kod som forts�tter k�ra rakt tills bilen passerat korsningen*/
						//N�r korsningen �r passerad , order = 0;
					}
				}

			
			}
		
		
		}/*END OF CROSSING = TRUE*/
	
	
	}/*END OF COMMAND*/




}/*END OF NAMESPACE Autodrive*/