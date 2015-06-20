#include "stdafx.h"

#include "Passenger.h"

// Static methods
DWORD WINAPI Passenger::ThreadWork(LPVOID parameter)
{
	PassengerParameter* passengerParameter = (PassengerParameter*)parameter;
	Passenger* passenger = ((Passenger*)passengerParameter->Object);

	printf("[Passenger %05d]  Running on PID:%i\r\n\r\n", GetCurrentThreadId(), GetCurrentProcessorNumber());

	DWORD ElevatorIsAvailableEventResponse;

	while (true)
	{
		// Waiting for elevator
		if (passenger->InElevator == nullptr)
		{
			// Passenger is at the floor he requested
			if (passenger->TargetFloor == passenger->Floor)
			{
				// Drink coffee for a bit...
				Sleep(2500);
				int randomRequest = passenger->Floor;
				do
				{
					randomRequest = rand() % 10;
				} while (randomRequest == passenger->Floor);

				passenger->SendRequest(randomRequest);
				ElevatorIsAvailableEventResponse = WaitForSingleObject(passengerParameter->FloorEvents[passenger->Floor], INFINITE);
			}

			switch (ElevatorIsAvailableEventResponse)
			{
				case WAIT_OBJECT_0:
					ResetEvent(passengerParameter->FloorEvents[passenger->Floor]);

					for (std::list<Elevator*>::iterator it = (passengerParameter->ElevatorList)->begin(); it != passengerParameter->ElevatorList->end(); ++it)
					{
						printf("[Passenger %05d]  Enters elevator\r\n\r\n", GetCurrentThreadId());

						((Elevator*)*it)->PassengerList.push_back(passenger);
						passenger->InElevator = *it;
					}
					break;
				default:
					printf("Wait error: (%d)\r\n", GetLastError());
					break;
			}
		}
		else
		{
			// "Exit" the elevator
			if (passenger->TargetFloor == passenger->Floor)
			{
				passenger->InElevator->PassengerList.remove(passenger);
				passenger->InElevator = nullptr;
				printf("[Passenger %05d]  Exits elevator\r\n\r\n", GetCurrentThreadId());
			}
		}
	}

	// Wait for elevator to arrive at current floor
		
	
	return 0;
}

Passenger::Passenger(const PassengerParameter& rhs)
	: ElevatableEntity(rhs)
	, InElevator(false)
{
	CreateThread(0, 2048, Passenger::ThreadWork, (LPVOID)&Parameter, 0, &AssociatedThreadID);
}

Passenger::~Passenger()
{

}

void Passenger::SendRequest(const int floor)
{
	if (Parameter.OpenRequestsMutex->try_lock())
	{
		printf("[Passenger %05d]  Requesting elevator from %i to %i\r\n", GetCurrentThreadId(), Floor, floor);
		TargetFloor = floor;
		Parameter.OpenRequests->push(FloorRequest(Floor, floor));
		Parameter.OpenRequestsMutex->unlock();
	}
}