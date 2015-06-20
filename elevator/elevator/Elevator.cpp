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
		if (elevatorPtr->currentRequest == FloorRequest::NoRequest)
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
				if (elevatorPtr->Floor == elevatorPtr->currentRequest.StartFloor)
				{
					elevatorPtr->SetFloor(elevatorPtr->Floor, true);
				}

				while (elevatorPtr->Floor != elevatorPtr->currentRequest.StartFloor)
				{
					elevatorPtr->SetFloor(
						elevatorPtr->Floor > elevatorPtr->currentRequest.StartFloor ?
						-1 :
						1
					);
					Sleep(500);
				}
			}
			while (elevatorPtr->Floor != elevatorPtr->currentRequest.TargetFloor)
			{
				elevatorPtr->SetFloor(
					elevatorPtr->Floor > elevatorPtr->currentRequest.TargetFloor ?
					-1 :
					1
				);
				for (int i = 0; i < elevatorPtr->PassengerList.size(); i++)
				{
					for (std::list<Passenger*>::iterator it = (elevatorPtr->PassengerList).begin(); it != (elevatorPtr->PassengerList).end(); ++it)
					{
						((Passenger*)*it)->SetFloor(elevatorPtr->Floor);
					}
				}
				Sleep(500);
			}

			elevatorPtr->currentRequest = FloorRequest::NoRequest;
		}
		// Do elevator stuff
	}

	Sleep(5);

	return 0;
}

Elevator::Elevator() : ElevatableEntity()
{
}

Elevator::Elevator(const ElevatorParameter& rhs) : ElevatableEntity(rhs)
{
	PassengerList = std::list<Passenger*>();
	CreateThread(0, 2048, Elevator::ThreadWork, (LPVOID)&Parameter, 0, &AssociatedThreadID);
}

Elevator::~Elevator()
{

}

int Elevator::SetFloor(const int floor, bool absolute)
{
	// Locally set new position
	Floor = absolute ? floor : Floor + floor;

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
		currentRequest = Parameter.OpenRequests->front();
		Parameter.OpenRequests->pop();
		currentRequest.AssociatedElevator = this;
		printf("[Elevator  %05d]  Now driving to %i\r\n", GetCurrentThreadId(), currentRequest.TargetFloor);
	}
}