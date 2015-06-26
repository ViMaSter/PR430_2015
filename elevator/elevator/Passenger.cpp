#include "stdafx.h"

#include "Passenger.h"

// Static methods
DWORD WINAPI Passenger::ThreadWork(LPVOID parameter)
{
	PassengerParameter* passengerParameter = (PassengerParameter*)parameter;
	Passenger* passenger = ((Passenger*)passengerParameter->Object);

	printf("[Passenger %05d]  Running on PID:%i\r\n\r\n", GetCurrentThreadId(), GetCurrentProcessorNumber());

	DWORD ElevatorIsAvailableEventResponse;

	bool InitialLoop = true;

	while (true)
	{
		// Waiting for elevator
		if (passenger->InElevator == nullptr)
		{
			// Passenger is at the floor he requested
			if (passenger->TargetFloor == passenger->Floor)
			{
				// Drink coffee for a bit...
				//if (!InitialLoop) {
					Sleep(2500);
				//}
				//else {
					//InitialLoop = false;
				//}

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

					printf("[Passenger %05d]  Reacting to floor event\r\n\r\n", GetCurrentThreadId());
					for (std::list<Elevator*>::iterator it = (passengerParameter->ElevatorList)->begin(); it != passengerParameter->ElevatorList->end(); ++it)
					{
						Elevator* currentElevator = ((Elevator*)*it);
						if (!currentElevator->CurrentlyMoving && (currentElevator->CurrentRequest.TargetFloor == passenger->TargetFloor)) {
							// Check if the elevator we're currently checking is handling the process of this passenger
							// and not moving
							printf("[Passenger %05d]  Enters elevator\r\n\r\n", GetCurrentThreadId());

							bool success = false;
							do {
								success = currentElevator->PassengerListMutex.try_lock();
							} while (!success);
							currentElevator->PassengerList.push_back(passenger);
							currentElevator->PassengerListMutex.unlock();

							passenger->InElevator = *it;
							break;
						}
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
				bool success = false;
				do {
					success = passenger->InElevator->PassengerListMutex.try_lock();
				} while (!success);
				passenger->InElevator->PassengerList.remove(passenger);
				passenger->InElevator->PassengerListMutex.unlock();
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
	bool success = false;
	do {
		success = Parameter.OpenRequestsMutex->try_lock();
	} while (!success);
	
	printf("[Passenger %05d]  Requesting elevator from %i to %i\r\n", GetCurrentThreadId(), Floor, floor);
	TargetFloor = floor;
	Parameter.OpenRequests->push(FloorRequest(Floor, floor));
	Parameter.OpenRequestsMutex->unlock();
}