#include "../Public/stdafx.h"

#include "isExport.h"

#include "FloorRequest.h"

const FloorRequest FloorRequest::NoRequest = FloorRequest();

// Only publicly to allow usage of std::queue with this type
FloorRequest::FloorRequest()
	: StartFloor(-1)
	, TargetFloor(-1)
	, AssociatedElevator(nullptr) {
}
FloorRequest::FloorRequest(const int startFloor, const int targetFloor)
	: StartFloor(startFloor)
	, TargetFloor(targetFloor)
	, AssociatedElevator(nullptr) {
}
FloorRequest::FloorRequest(const FloorRequest& rhs)
	: StartFloor(rhs.StartFloor)
	, TargetFloor(rhs.TargetFloor)
	, AssociatedElevator(rhs.AssociatedElevator) {
}

FloorRequest& FloorRequest::operator=(const FloorRequest& rhs) {
	StartFloor = rhs.StartFloor;
	TargetFloor = rhs.TargetFloor;
	AssociatedElevator = rhs.AssociatedElevator;
	return *this;
}

bool FloorRequest::operator==(const FloorRequest& rhs) {
	return this->TargetFloor == rhs.TargetFloor;
}

void FloorRequest::AssociateElevator(Elevator* elevator) {
	AssociatedElevator = elevator;
}