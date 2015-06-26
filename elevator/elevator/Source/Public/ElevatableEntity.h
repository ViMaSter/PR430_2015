#pragma once

#include "windows.h"

#include <map>
#include <list>
#include <queue>
#include <mutex>

#include "export.h"

#include "FloorRequest.h"

class Passenger;
class Elevator;
class ElevatableEntity;

struct ELEVATOR_API ElevatableEntityParameter
{
private:
	ElevatableEntityParameter& operator=(const ElevatableEntityParameter& rhs) = delete;

public:
	const int StartFloor;

	HANDLE* FloorEvents;
	std::mutex* OpenRequestsMutex;
	std::queue<FloorRequest*>* OpenRequests;

	std::list<Elevator*>* ElevatorList;
	std::list<Passenger*>* PassengerList;

	ElevatableEntity* Object;

	ElevatableEntityParameter();
	ElevatableEntityParameter(int startFloor, HANDLE* floorEvents, std::mutex* openRequestsMutex, std::queue<FloorRequest*>* openRequests, std::list<Elevator*>* elevatorList, std::list<Passenger*>* passengerList);
	ElevatableEntityParameter(const ElevatableEntityParameter& rhs);
};

class ELEVATOR_API ElevatableEntity
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

