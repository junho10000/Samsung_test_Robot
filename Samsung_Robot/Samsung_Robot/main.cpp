#include <iostream>

using namespace std;

#define ClOCKWISE 1
#define COUNTERCLOCKWISE 2



struct command
{
	char direction;
	int clock_dir;
	int spinNum;
};
struct spindles
{
	int mySize;
	int *myData;
};

struct maps
{
	int row;
	int col;
	int **mapData;
};

class Robot
{
private:
	int X;
	int Y;
	command *robotCommand;
	int robotCommandSize;
	int **robotMap;

public:
	//Robot(int, int, command*, int);
	Robot();
	void RobotInit(int, int, command*, int);
	void getRobot();
	void doCommand(maps, spindles);
	
};

void printMap(maps myMap)
{
	for (int i = 0; i < myMap.row; i++)
	{
		for (int j = 0; j < myMap.col; j++)
		{
			cout << myMap.mapData[i][j] << " ";
		}
		cout << endl;
	}
}

int main()
{
	/*
		input value
		testCase
	*/
	int testCase;
	cin >> testCase;
	/*
		init
		robot, maps
	*/
	Robot *robot = new Robot[testCase];
	maps *myMap = new maps[testCase];
	spindles *mySpindle = new spindles[testCase];
	
	/*
		Loop testCase times
	*/
	for (int t = 0; t < testCase; t++)
	{
		/*
			input
			row, col, startPoint: x, y
		*/
		int row, col, x, y;
		cin >> myMap[t].col >> myMap[t].row >> x >> y;

		/*
			Initialize map
		*/
		myMap[t].mapData = new int *[myMap[t].row];
		for (int i = 0; i < myMap[t].row; i++)
		{
			myMap[t].mapData[i] = new int[myMap[t].col];
		}

		for (int i = 0; i < myMap[t].row; i++)
		{
			for (int j = 0; j < myMap[t].col; j++)
			{
				cin >> myMap[t].mapData[i][j];
			}
		}
		/*
			input
			spindleSize, spinde
		*/
		cin >> mySpindle[t].mySize;
		mySpindle[t].myData = new int[mySpindle[t].mySize];

		for (int i = 0; i < mySpindle[t].mySize; i++)
		{
			cin >> mySpindle[t].myData[i];
		}
		/*
			input
			commandSize, command
		*/
		int commandSize;
		cin >> commandSize;
		command *myCommand = new command[commandSize];
		for (int i = 0; i < commandSize; i++)
		{
			cin >> myCommand[i].direction >> myCommand[i].clock_dir >> myCommand[i].spinNum;
		}
		/*
			Init
			robot's value
		*/
		robot[t].RobotInit(x, y, myCommand, commandSize);
		

	}
	/*
		do each robot's command
	*/
	for (int i = 0; i < testCase; i++)
	{
		cout << "#" << i + 1 << " ";
		robot[i].doCommand(myMap[i], mySpindle[i]);
		cout << endl;
	}
	system("pause");
	return 0;
}

//Robot::Robot(int X, int Y, command *robotCommand, int robotCommandSize)
//{
//	this->X = X;
//	this->Y = Y;
//	this->robotCommand = new command[robotCommandSize];
//	this->robotCommandSize = robotCommandSize;
//	for (int i = 0; i < robotCommandSize; i++)
//	{
//		this->robotCommand[i] = robotCommand[i];
//	}
//}

Robot::Robot()
{
}

void Robot::RobotInit(int X, int Y, command *robotCommand, int robotCommandSize)
{
	//to use map  as 0 ~ N 
	this->X = X - 1;
	this->Y = Y - 1;
	this->robotCommand = new command[robotCommandSize];
	this->robotCommandSize = robotCommandSize;
	for (int i = 0; i < robotCommandSize; i++)
	{
		this->robotCommand[i] = robotCommand[i];
	}
}

void Robot::getRobot()
{
	for (int i = 0; i < robotCommandSize; i++)
	{
		cout << robotCommand[i].direction<<endl;
	}
}

void Robot::doCommand(maps myMap, spindles mySpindle)
{
	/*
		1. spin the spindle
		2. get move point
		3. move robot in command's direction
		4. plus myMap data to myPoint
		5. change myMap data --> 0
		5.1 if myMap data is -1(obstacle) stop
		5.2 if out of range stop
	*/
	int movePoint;
	int spinIndex = 0;
	int myPoint = myMap.mapData[Y][X];
	myMap.mapData[Y][X] = 0;
	//do each command
	for (int i = 0; i < robotCommandSize; i++)
	{
		/*
			1. spin the spindle
			2. get move point
		*/
		int spinPoint = robotCommand[i].spinNum % mySpindle.mySize;
		if (spinPoint == 0)
		{
			movePoint = mySpindle.myData[spinIndex];
		}
		else if (robotCommand[i].clock_dir == ClOCKWISE)
		{
			spinIndex = (spinIndex - spinPoint) % mySpindle.mySize;
			if (spinIndex < 0)
			{
				spinIndex = mySpindle.mySize + spinIndex;
			}
			movePoint = mySpindle.myData[spinIndex];
		}
		else if (robotCommand[i].clock_dir == COUNTERCLOCKWISE)
		{
			spinIndex = (spinIndex + spinPoint) % mySpindle.mySize;
			if (spinIndex >= mySpindle.mySize)
			{
				spinIndex = spinIndex - mySpindle.mySize;
			}
			movePoint = mySpindle.myData[spinIndex];
		}
		/*
			3. move robot in command's direction
		*/
		
		switch (robotCommand[i].direction)
		{
		case 'N':
			while (movePoint != 0)
			{
				movePoint--;
				Y--;
				if (Y < 0)
				{
					Y++;
					break;
				}

				if (myMap.mapData[Y][X] == -1)
				{
					Y++;
					break;
				}
				myPoint += myMap.mapData[Y][X];
				myMap.mapData[Y][X] = 0;
			}
			break;
		case 'S':
			while (movePoint != 0)
			{
				movePoint--;
				Y++;
				if (Y >= myMap.row)
				{
					Y--;
					break;
				}

				if (myMap.mapData[Y][X] == -1)
				{
					Y--;
					break;
				}
				myPoint += myMap.mapData[Y][X];
				myMap.mapData[Y][X] = 0;
			}
			break;
		case 'W':
			while (movePoint != 0)
			{
				movePoint--;
				X--;
				if (X < 0)
				{
					X++;
					break;
				}

				if (myMap.mapData[Y][X] == -1)
				{
					X++;
					break;
				}
				myPoint += myMap.mapData[Y][X];
				myMap.mapData[Y][X] = 0;
			}

			break;
		case 'E':
			while (movePoint != 0)
			{
				movePoint--;
				X++;
				if (X >= myMap.col)
				{
					X--;
					break;
				}

				if (myMap.mapData[Y][X] == -1)
				{
					X--;
					break;
				}
				myPoint += myMap.mapData[Y][X];
				myMap.mapData[Y][X] = 0;
			}
			break;
		}

		//printMap(myMap);
		/*cout << "My Point: " << myPoint << endl;
		cout << "X: " << X << " Y: " << Y << endl;*/
		//system("pause");
	}
	cout << myPoint << " " << X + 1 << " " << Y + 1;

}
