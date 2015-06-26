#pragma once

#include "ElevatableEntity.h"

#include "ConsoleBufferHelper.h"

#include "windows.h"

struct ELEVATOR_API ElevatorParameter : public ElevatableEntityParameter
{
private:
	ElevatorParameter();
public:
	ElevatorParameter(int startFloor, HANDLE* floorEvents, std::mutex* openRequestsMutex, std::queue<FloorRequest*>* openRequests, std::list<Elevator*>* elevatorList, std::list<Passenger*>* passengerList);
	ElevatorParameter(const ElevatableEntityParameter& rhs);
};

class Passenger;
class ELEVATOR_API Elevator : public ElevatableEntity
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

	FloorRequest* CurrentRequest;

	// Passengers only enter an elevator when they announce themselves on a floor and are not CurrentlyMoving
	bool CurrentlyMoving;
	std::mutex* PassengerListMutex;
	std::list<Passenger*>* PassengerList;

	// ctor
	Elevator();
	Elevator(const ElevatorParameter& rhs);
	// dtor
	virtual ~Elevator();

	void Clear();
	void Render();

	virtual int SetFloor(const int floor, bool absolute = false);

	// Returns true, when the elevator successfully took a new request
	bool TakeNextRequest();
};