#pragma once

#include "windows.h"

#include "Elevator.h"
#include "ElevatableEntity.h"

#include "export.h"

struct ELEVATOR_API PassengerParameter : public ElevatableEntityParameter
{
private:
	PassengerParameter();
public:
	PassengerParameter(int startFloor, HANDLE* floorEvents, std::mutex* openRequestsMutex, std::queue<FloorRequest*>* openRequests, std::list<Elevator*>* elevatorList, std::list<Passenger*>* passengerList);
	PassengerParameter(const ElevatableEntityParameter& rhs);
};

class ELEVATOR_API Passenger : public ElevatableEntity
{
private:
	// ID stuff
	static int nextID;
public:
	static int GetNextID();

	int ID;

public:
	// static
	static DWORD WINAPI ThreadWork(LPVOID parameter);

	// instance data
	FloorRequest* CurrentRequest;
	Elevator* InElevator;

	int TargetFloor = 0;

	// ctor
	Passenger(const PassengerParameter& rhs);
	// dtor
	virtual ~Passenger();

	void Clear();
	void Render();

	virtual int SetFloor(const int floor);

	void SendRequest(const int floor);
};