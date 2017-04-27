#include "autodrive.hpp"
#include "sensordata.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include "../imageprocessor/imageprocessor.hpp"

/*
Kör tills irDownLeft/Right upptäcker att en rak linje ------


*/
using namespace cv;

namespace Autodrive {
	
	int usFront;
	int usFrontRight;
	int irDownRight;
	int irDownLeft;

	int order = 0;

	bool crossing = false; // Aktivera när vi vill köra i korsning
	bool crossingLineEnter = false; // linjen precis innan korsningen (Global)
	
	/*Variabler för vägar bilen kan ta i en korsning*/
	int leftTurn = 0;
	int rightTurn = 0;
	int moveStraight = 0;

	command run(command lastCommand, Mat *Mat)
	{
		usFront = SensorData::ultrasound.front;
		usFrontRight = SensorData::ultrasound.frontRight; // ändra i sensordata
		irDownLeft = SensorData::infrared.frontLeft;
		irDownRight = SensorData::infrared.frontRight;
		//irActive = 0;

		lastCommand.setSpeed(0.4);//Ändra farten(om det behövs)

		/*Om ir sensorerna under bilen upptäcker att vi kör över korsningsaktiverande
		linje*/
		if (irDownLeft == 1 || irDownRight == 1) 
		{
			//irActive = 1;
			crossing = true;
			if (!crossingLineEnter) 
			{
				crossingLineEnter = true;// glöm inte att ändra till false när man kört ut från korsning
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
			crossing = false; // kanske? eller crossing = false när korsningscenariot är slut?
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
					while (1) //&& andra bilen inte är i crossing mode
					{
						/*Någon kod som fortsätter köra rakt tills bilen passerat korsningen*/
						//När korsningen är passerad , order = 0;
					}
				}

			
			}
		
		
		}/*END OF CROSSING = TRUE*/
	
	
	}/*END OF COMMAND*/




}/*END OF NAMESPACE Autodrive*/