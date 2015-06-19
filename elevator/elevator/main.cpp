// elevator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <vector>

#include "Elevator.h"
#include "Passenger.h"

int _tmain(int argc, _TCHAR* argv[])
{
	// Initialize member
	ElevatorParameter FirstElevatorParameter = ElevatorParameter(0);
	PassengerParameter FirstPassengerParameter = PassengerParameter(0);
	Elevator FirstElevator = Elevator(FirstElevatorParameter);
	Passenger FirstPassenger = Passenger(FirstPassengerParameter);

	std::vector<int> ElevatorThreadIDs;
	std::vector<int> PassengerThreadIDs;

	return 0;
}

