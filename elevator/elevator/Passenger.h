#pragma once

#include "windows.h"

struct PassengerParameter
{
private:
	PassengerParameter& operator=(PassengerParameter& rhs)
	{
	}

public:
	const int StartFloor;
	PassengerParameter() : StartFloor(0)
	{
	}
	PassengerParameter(int startFloor) : StartFloor(startFloor)
	{
	}

	PassengerParameter(const PassengerParameter& rhs) : StartFloor(rhs.StartFloor)
	{
		
	}
};

class Passenger
{
private:
	Passenger();
	int Floor;

public:
	Passenger(const PassengerParameter& rhs);
	~Passenger();

	PassengerParameter Parameter;
	DWORD AssociatedThreadID;

	const int GetFloor() const;
	int SetFloor(int floor);
	static DWORD WINAPI ThreadWork(LPVOID parameter);
};

