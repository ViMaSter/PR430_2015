#include "../Public/stdafx.h"

#include "isExport.h"
#include "Passenger.h"
#include "Elevator.h"

ElevatorParameter::ElevatorParameter() : ElevatableEntityParameter() {
}
ElevatorParameter::ElevatorParameter(int startFloor, HANDLE* floorEvents, std::mutex* openRequestsMutex, std::queue<FloorRequest*>* openRequests, std::list<Elevator*>* elevatorList, std::list<Passenger*>* passengerList)
	: ElevatableEntityParameter(startFloor, floorEvents, openRequestsMutex, openRequests, elevatorList, passengerList) {
}

ElevatorParameter::ElevatorParameter(const ElevatableEntityParameter& rhs)
	: ElevatableEntityParameter(rhs) {
}

int Elevator::GetNextID()
{
	int ID = Elevator::nextID;
	Elevator::nextID++;
	return ID;
}

// Static methods
DWORD WINAPI Elevator::ThreadWork(LPVOID parameter)
{
	ElevatorParameter* elevatorParameter = (ElevatorParameter*)parameter;
	Elevator* elevatorPtr = ((Elevator*)(elevatorParameter->Object));

	//printf("[Elevator  %05d]  Starting - Running on PID:%i\r\n\r\n", GetCurrentThreadId(), GetCurrentProcessorNumber());

	elevatorPtr->SetFloor(elevatorParameter->StartFloor, true);

	while (true/* insert any optional condition to stop the elevator */)
	{
		// Loop until the elevator has a request
		if (elevatorPtr->CurrentRequest == nullptr)
		{
			// @Notice:	Improve by only calling this, when a new
			//			request is added and sleep until then
			if (elevatorParameter->OpenRequestsMutex->try_lock())
			{
				if (((Elevator*)(elevatorParameter->Object))->TakeNextRequest()) {
					elevatorPtr->SetFloor(elevatorPtr->Floor, true);
				}
				elevatorParameter->OpenRequestsMutex->unlock();
			}
		}
		else
		{
			// Logic ran when we have a request but not picked up the customer yet
			while (elevatorPtr->PassengerList->empty())
			{
				//	If the elevator has a request and is empty
				//  and we arrive at the StartFloor of our request...
				if (elevatorPtr->Floor == elevatorPtr->CurrentRequest->StartFloor)
				{
					// ...we announce ourselves with CurrentlyMoving = false...
					elevatorPtr->CurrentlyMoving = false;
					elevatorPtr->SetFloor(elevatorPtr->Floor, true);
					while (elevatorPtr->PassengerList->empty()) {
						// ...and wait until we're no longer empty
						Sleep(5);
					}
				}

				// If we're not at the start floor (and still empty)
				while (elevatorPtr->Floor != elevatorPtr->CurrentRequest->StartFloor)
				{
					elevatorPtr->CurrentlyMoving = true;
					// ...we move closer to the start floor
					elevatorPtr->SetFloor(
						elevatorPtr->Floor > elevatorPtr->CurrentRequest->StartFloor ?
						-1 :
						1
					);
					Sleep(500);
				}
			}

			// Logic ran when we have a request and picked up the customer
			while (elevatorPtr->Floor != elevatorPtr->CurrentRequest->TargetFloor)
			{
				elevatorPtr->CurrentlyMoving = true;
				// We move closer to the target floor
				elevatorPtr->SetFloor(
					elevatorPtr->Floor > elevatorPtr->CurrentRequest->TargetFloor ?
					-1 :
					1
				);
				Sleep(500);
			}

			// And reset the elevator after that
			elevatorPtr->CurrentRequest = nullptr;
		}
	}

	return 0;
}

Elevator::Elevator()
	: ElevatableEntity()
	, CurrentlyMoving(false)
{
	ID = -1;
	CurrentRequest = nullptr;
}

Elevator::Elevator(const ElevatorParameter& rhs)
	: ElevatableEntity(rhs)
	, CurrentlyMoving(false)
{
	ID = GetNextID();
	CurrentRequest = nullptr;
	PassengerListMutex = new std::mutex();
	PassengerList = new std::list<Passenger*>();
	CreateThread(0, 2048, Elevator::ThreadWork, (LPVOID)&Parameter, 0, &AssociatedThreadID);
}

Elevator::~Elevator()
{
}

void Elevator::Render() {
	COORD c;
	c.X = ID;
	c.Y = Floor;
	ConsoleBufferHelper::JumpToPosition(c);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
	printf("E");
	ConsoleBufferHelper::GetMutex()->unlock();
}

void Elevator::Clear() {
	COORD c;
	c.X = ID;
	c.Y = Floor;
	ConsoleBufferHelper::JumpToPosition(c);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
	printf(" ");
	ConsoleBufferHelper::GetMutex()->unlock();
}

int Elevator::SetFloor(const int floor, const bool absolute)
{
	Clear();

	// Locally set new position
	Floor = absolute ? floor : Floor + floor;

	// Set floor for all passengers

	bool success = false;
	do {
		success = PassengerListMutex->try_lock();
	} while (!success);

	for (std::list<Passenger*>::iterator it = PassengerList->begin(); it != PassengerList->end(); ++it) {
		((Passenger*)*it)->SetFloor(Floor);
	}

	Render();

	PassengerListMutex->unlock();

	// Announce the new floor this elevator is at
	if (!SetEvent(Parameter.FloorEvents[Floor]))
	{
		//printf("[Elevator  %05d]  ERROR: Calling floor event on level %i failed\r\n", GetCurrentThreadId(), Floor);
	}
	else
	{
		//printf("[Elevator  %05d]  Announcement: Elevator now on floor %i\r\n\r\n", GetCurrentThreadId(), Floor);
	}

	return Floor;
}

bool Elevator::TakeNextRequest()
{
	if (!Parameter.OpenRequests->empty())
	{
		CurrentRequest = Parameter.OpenRequests->front();
		Parameter.OpenRequests->pop();
		CurrentRequest->AssociatedElevator = this;
		//printf("[Elevator  %05d]  Now driving to %i\r\n", GetCurrentThreadId(), CurrentRequest->TargetFloor);
		return true;
	}
	return false;
}

int Elevator::nextID = 0;