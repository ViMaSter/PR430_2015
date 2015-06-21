#include "stdafx.h"

#include "Elevator.h"
#include "Passenger.h"

// Static methods
DWORD WINAPI Elevator::ThreadWork(LPVOID parameter)
{
	ElevatorParameter* elevatorParameter = (ElevatorParameter*)parameter;
	Elevator* elevatorPtr = ((Elevator*)(elevatorParameter->Object));

	printf("[Elevator  %05d]  Starting - Running on PID:%i\r\n\r\n", GetCurrentThreadId(), GetCurrentProcessorNumber());

	elevatorPtr->SetFloor(elevatorParameter->StartFloor, true);

	while (true/* insert any optional condition to stop the elevator */)
	{
		if (elevatorPtr->CurrentRequest == FloorRequest::NoRequest)
		{
			// @Notice:	Improve by calling when a new request is added and do "nothing" while "nothing" is
			//			happening instead of checking every time and locking the request all the time
			if (elevatorParameter->OpenRequestsMutex->try_lock())
			{
				((Elevator*)(elevatorParameter->Object))->TakeNextRequest();
				elevatorParameter->OpenRequestsMutex->unlock();
			}
		}
		else
		{
			while (elevatorPtr->PassengerList.empty())
			{
				elevatorPtr->CurrentlyMoving = true;
				if (elevatorPtr->Floor == elevatorPtr->CurrentRequest.StartFloor)
				{
					elevatorPtr->CurrentlyMoving = false;
					elevatorPtr->SetFloor(elevatorPtr->Floor, true);
					while (elevatorPtr->PassengerList.empty()) {
						Sleep(5);
					}
				}

				while (elevatorPtr->Floor != elevatorPtr->CurrentRequest.StartFloor)
				{
					elevatorPtr->SetFloor(
						elevatorPtr->Floor > elevatorPtr->CurrentRequest.StartFloor ?
						-1 :
						1
					);
					if (elevatorPtr->Floor == elevatorPtr->CurrentRequest.StartFloor) {
						elevatorPtr->CurrentlyMoving = false;
					}
					Sleep(500);
				}
			}
			elevatorPtr->CurrentlyMoving = true;
			while (elevatorPtr->Floor != elevatorPtr->CurrentRequest.TargetFloor)
			{
				elevatorPtr->SetFloor(
					elevatorPtr->Floor > elevatorPtr->CurrentRequest.TargetFloor ?
					-1 :
					1
				);
				elevatorPtr->CurrentlyMoving = false;
				Sleep(500);
			}

			elevatorPtr->CurrentRequest = FloorRequest::NoRequest;
		}
		// Do elevator stuff
	}

	Sleep(5);

	return 0;
}

Elevator::Elevator()
	: ElevatableEntity()
	, CurrentlyMoving(false)
{
}

Elevator::Elevator(const ElevatorParameter& rhs)
	: ElevatableEntity(rhs)
	, CurrentlyMoving(false)
{
	PassengerList = std::list<Passenger*>();
	CreateThread(0, 2048, Elevator::ThreadWork, (LPVOID)&Parameter, 0, &AssociatedThreadID);
}

Elevator::~Elevator()
{

}

int Elevator::SetFloor(const int floor, const bool absolute)
{
	// Locally set new position
	Floor = absolute ? floor : Floor + floor;

	// Set floor for all passengers

	bool success = false;
	do {
		success = PassengerListMutex.try_lock();
	} while (!success);

	for (std::list<Passenger*>::iterator it = PassengerList.begin(); it != PassengerList.end(); ++it) {
		((Passenger*)*it)->SetFloor(Floor);
	}

	PassengerListMutex.unlock();

	// Announce the new floor this elevator is at
	if (!SetEvent(Parameter.FloorEvents[Floor]))
	{
		printf("[Elevator  %05d]  ERROR: Calling floor event on level %i failed\r\n", GetCurrentThreadId(), Floor);
	}
	else
	{
		printf("[Elevator  %05d]  Announcement: Elevator now on floor %i\r\n\r\n", GetCurrentThreadId(), Floor);
	}

	return Floor;
}

void Elevator::TakeNextRequest()
{
	if (!Parameter.OpenRequests->empty())
	{
		CurrentRequest = Parameter.OpenRequests->front();
		Parameter.OpenRequests->pop();
		CurrentRequest.AssociatedElevator = this;
		printf("[Elevator  %05d]  Now driving to %i\r\n", GetCurrentThreadId(), CurrentRequest.TargetFloor);
	}
}