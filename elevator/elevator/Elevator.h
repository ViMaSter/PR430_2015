#pragma once

#include "ElevatableEntity.h"

#include "windows.h"

struct ElevatorParameter : public ElevatableEntityParameter
{
private:
	ElevatorParameter() : ElevatableEntityParameter()
	{
	}
public:
	ElevatorParameter(int startFloor, HANDLE* floorEvents, std::mutex* openRequestsMutex, std::queue<FloorRequest>* openRequests, std::list<Elevator*>* elevatorList, std::list<Passenger*>* passengerList)
		: ElevatableEntityParameter(startFloor, floorEvents, openRequestsMutex, openRequests, elevatorList, passengerList)
	{
	}

	ElevatorParameter(const ElevatableEntityParameter& rhs)
		: ElevatableEntityParameter(rhs)
	{
	}
};

class Passenger;
class Elevator : public ElevatableEntity
{
public:
	// static
	static DWORD WINAPI ThreadWork(LPVOID parameter);

	FloorRequest CurrentRequest;
	bool CurrentlyMoving;
	std::mutex PassengerListMutex;
	std::list<Passenger*> PassengerList;

	// ctor
	Elevator();
	Elevator(const ElevatorParameter& rhs);
	// dtor
	virtual ~Elevator();

	virtual int SetFloor(const int floor, bool absolute = false);

	void TakeNextRequest();
};