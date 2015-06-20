#pragma once

class Elevator;

struct FloorRequest
{
public:
	static const FloorRequest NoRequest;

	// Only public to allow usage of std::queue with this type
	FloorRequest()
		: StartFloor(-1)
		, TargetFloor(-1)
		, AssociatedElevator(nullptr)
	{}
	FloorRequest(const int startFloor, const int targetFloor)
		: StartFloor(startFloor)
		, TargetFloor(targetFloor)
		, AssociatedElevator(nullptr)
	{
	}
	FloorRequest(const FloorRequest& rhs)
		: StartFloor(rhs.StartFloor)
		, TargetFloor(rhs.TargetFloor)
		, AssociatedElevator(rhs.AssociatedElevator)
	{
	}

	FloorRequest& operator=(const FloorRequest& rhs)
	{
		StartFloor = rhs.StartFloor;
		TargetFloor = rhs.TargetFloor;
		AssociatedElevator = rhs.AssociatedElevator;
		return *this;
	}

	bool operator==(const FloorRequest& rhs)
	{
		return this->TargetFloor == rhs.TargetFloor;
	}

	int StartFloor;
	int TargetFloor;

	Elevator* AssociatedElevator;

	void AssociateElevator(Elevator* elevator)
	{
		AssociatedElevator = elevator;
	}
};