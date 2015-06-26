// elevatorDLLUser.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Elevator.h"
#include "Passenger.h"

int _tmain(int argc, _TCHAR* argv[])
{
	// Initialize queue and map
	const int ElevatorCount = 5;
	const int PassengerCount = 5;
	const int FloorCount = 10;
	HANDLE FloorEvents[FloorCount];

	for (int i = 0; i < FloorCount; i++) {
		FloorEvents[i] = CreateEvent(NULL, true, false, NULL);
	}

	std::mutex OpenRequestsMutex;
	std::queue<FloorRequest*> OpenRequests = std::queue<FloorRequest*>();

	std::list<Elevator*> ElevatorList = std::list<Elevator*>();
	std::list<Passenger*> PassengerList = std::list<Passenger*>();

	// Initialize entities
	ElevatorParameter SampleElevatorParameter = ElevatorParameter(2, FloorEvents, &OpenRequestsMutex, &OpenRequests, &ElevatorList, &PassengerList);
	PassengerParameter SamplePassengerParameter = PassengerParameter(0, FloorEvents, &OpenRequestsMutex, &OpenRequests, &ElevatorList, &PassengerList);

	for (int i = 0; i < ElevatorCount; i++) {
		ElevatorList.push_back(new Elevator(SampleElevatorParameter));
	}
	for (int i = 0; i < PassengerCount; i++) {
		PassengerList.push_back(new Passenger(SamplePassengerParameter));
	}

	while (true) {
		Sleep(10);
	}

	for (int i = 0; i < ElevatorCount; i++) {
		delete ElevatorList.back();
		ElevatorList.pop_back();
	}

	for (int i = 0; i < PassengerCount; i++) {
		delete PassengerList.back();
		PassengerList.pop_back();
	}

	return 0;
}

