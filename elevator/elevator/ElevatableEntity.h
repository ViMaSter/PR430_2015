#pragma once

#include "windows.h"

#include <map>
#include <list>
#include <queue>
#include <mutex>

#include "FloorRequest.h"

class Passenger;
class Elevator;
class ElevatableEntity;

struct ElevatableEntityParameter
{
private:
	ElevatableEntityParameter& operator=(ElevatableEntityParameter& rhs)
	{
	}

public:
	const int StartFloor;

	HANDLE* FloorEvents;
	std::mutex* OpenRequestsMutex;
	std::queue<FloorRequest>* OpenRequests;

	std::list<Elevator*>* ElevatorList;
	std::list<Passenger*>* PassengerList;

	ElevatableEntity* Object;

	ElevatableEntityParameter()
		: StartFloor(0)
		, FloorEvents(nullptr)
		, OpenRequestsMutex(nullptr)
		, OpenRequests(nullptr)
		, Object(nullptr)
		, ElevatorList(nullptr)
		, PassengerList(nullptr)
	{
	}

	ElevatableEntityParameter(int startFloor, HANDLE* floorEvents, std::mutex* openRequestsMutex, std::queue<FloorRequest>* openRequests, std::list<Elevator*>* elevatorList, std::list<Passenger*>* passengerList)
		: StartFloor(startFloor)
		, FloorEvents(floorEvents)
		, OpenRequestsMutex(openRequestsMutex)
		, OpenRequests(openRequests)
		, Object(nullptr)
		, ElevatorList(elevatorList)
		, PassengerList(passengerList)
	{
	}

	ElevatableEntityParameter(const ElevatableEntityParameter& rhs)
		: StartFloor(rhs.StartFloor)
		, FloorEvents(rhs.FloorEvents)
		, OpenRequestsMutex(rhs.OpenRequestsMutex)
		, OpenRequests(rhs.OpenRequests)
		, Object(rhs.Object)
		, ElevatorList(rhs.ElevatorList)
		, PassengerList(rhs.PassengerList)
	{
	}
};

class ElevatableEntity
{
protected:
	ElevatableEntity();
	ElevatableEntity(const ElevatableEntityParameter& parameter);
	int Floor;

public:
	~ElevatableEntity();

	ElevatableEntityParameter Parameter;
	DWORD AssociatedThreadID;

	const int GetFloor() const;
	virtual int SetFloor(const int floor);
};

