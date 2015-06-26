#include "../Public/stdafx.h"

#include "isExport.h"

#include "ElevatableEntity.h"

ElevatableEntityParameter::ElevatableEntityParameter()
	: StartFloor(0)
	, FloorEvents(nullptr)
	, OpenRequestsMutex(nullptr)
	, OpenRequests(nullptr)
	, Object(nullptr)
	, ElevatorList(nullptr)
	, PassengerList(nullptr) {
}

ElevatableEntityParameter::ElevatableEntityParameter(int startFloor, HANDLE* floorEvents, std::mutex* openRequestsMutex, std::queue<FloorRequest*>* openRequests, std::list<Elevator*>* elevatorList, std::list<Passenger*>* passengerList)
	: StartFloor(startFloor)
	, FloorEvents(floorEvents)
	, OpenRequestsMutex(openRequestsMutex)
	, OpenRequests(openRequests)
	, Object(nullptr)
	, ElevatorList(elevatorList)
	, PassengerList(passengerList) {
}

ElevatableEntityParameter::ElevatableEntityParameter(const ElevatableEntityParameter& rhs)
	: StartFloor(rhs.StartFloor)
	, FloorEvents(rhs.FloorEvents)
	, OpenRequestsMutex(rhs.OpenRequestsMutex)
	, OpenRequests(rhs.OpenRequests)
	, Object(rhs.Object)
	, ElevatorList(rhs.ElevatorList)
	, PassengerList(rhs.PassengerList) {
}

// Instance methods
ElevatableEntity::ElevatableEntity(const ElevatableEntityParameter& parameter) : Parameter(parameter)
{
	Floor = Parameter.StartFloor;
	Parameter.Object = this;
}

ElevatableEntity::ElevatableEntity()
{
}

ElevatableEntity::~ElevatableEntity()
{
}

int ElevatableEntity::SetFloor(const int floor)
{
	Floor = floor;
	return Floor;
}

const int ElevatableEntity::GetFloor() const
{
	return Floor;
}
