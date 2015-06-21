#pragma once

#include "windows.h"

#include "Elevator.h"
#include "ElevatableEntity.h"

struct PassengerParameter : public ElevatableEntityParameter
{
private:
	PassengerParameter()
		: ElevatableEntityParameter()
	{
	}
public:
	PassengerParameter(int startFloor, HANDLE* floorEvents, std::mutex* openRequestsMutex, std::queue<FloorRequest>* openRequests, std::list<Elevator*>* elevatorList, std::list<Passenger*>* passengerList)
		: ElevatableEntityParameter(startFloor, floorEvents, openRequestsMutex, openRequests, elevatorList, passengerList)
	{
	}

	PassengerParameter(const ElevatableEntityParameter& rhs)
		: ElevatableEntityParameter(rhs)
	{
	}
};

class Passenger : public ElevatableEntity
{
public:
	// static
	static DWORD WINAPI ThreadWork(LPVOID parameter);

	// instance data
	Elevator* InElevator;
	FloorRequest* currentRequest;

	int TargetFloor = 0;

	// ctor
	Passenger(const PassengerParameter& rhs);
	// dtor
	virtual ~Passenger();

	void SendRequest(const int floor);
};

