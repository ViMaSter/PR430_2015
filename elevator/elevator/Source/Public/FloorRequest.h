#pragma once

class Elevator;

#include "export.h"

struct ELEVATOR_API FloorRequest
{
public:
	static const FloorRequest NoRequest;

	// Only public to allow usage of std::queue with this type
	FloorRequest();
	FloorRequest(const int startFloor, const int targetFloor);
	FloorRequest(const FloorRequest& rhs);

	FloorRequest& operator=(const FloorRequest& rhs);

	bool operator==(const FloorRequest& rhs);

	int StartFloor;
	int TargetFloor;

	Elevator* AssociatedElevator;

	void AssociateElevator(Elevator* elevator);
};