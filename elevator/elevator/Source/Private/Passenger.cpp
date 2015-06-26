#include "../Public/stdafx.h"

#include "isExport.h"

#include "Passenger.h"

PassengerParameter::PassengerParameter()
	: ElevatableEntityParameter() {
}

PassengerParameter::PassengerParameter(int startFloor, HANDLE* floorEvents, std::mutex* openRequestsMutex, std::queue<FloorRequest*>* openRequests, std::list<Elevator*>* elevatorList, std::list<Passenger*>* passengerList)
	: ElevatableEntityParameter(startFloor, floorEvents, openRequestsMutex, openRequests, elevatorList, passengerList) {
}

PassengerParameter::PassengerParameter(const ElevatableEntityParameter& rhs)
	: ElevatableEntityParameter(rhs) {
}

// Static methods
DWORD WINAPI Passenger::ThreadWork(LPVOID parameter)
{
	PassengerParameter* passengerParameter = (PassengerParameter*)parameter;
	Passenger* passenger = ((Passenger*)passengerParameter->Object);

	//printf("[Passenger %05d]  Running on PID:%i\r\n\r\n", GetCurrentThreadId(), GetCurrentProcessorNumber());

	DWORD ElevatorIsAvailableEventResponse;

	passenger->SetFloor(passenger->Floor);

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
					Sleep(2500+(rand()%500));
				//}
				//else {
					//InitialLoop = false;
				//}

				int randomRequest = passenger->Floor;
				do
				{
					randomRequest = (rand() * passenger->ID) % 10;
				} while (randomRequest == passenger->Floor);

				passenger->SendRequest(randomRequest);
				ElevatorIsAvailableEventResponse = WaitForSingleObject(passengerParameter->FloorEvents[passenger->Floor], INFINITE);
			}

			switch (ElevatorIsAvailableEventResponse)
			{
				case WAIT_OBJECT_0:
					ResetEvent(passengerParameter->FloorEvents[passenger->Floor]);

					////printf("[Passenger %05d]  Reacting to floor event\r\n\r\n", GetCurrentThreadId());
					for (std::list<Elevator*>::iterator it = (passengerParameter->ElevatorList)->begin(); it != passengerParameter->ElevatorList->end(); ++it)
					{
						Elevator* currentElevator = ((Elevator*)*it);
						if (!currentElevator->CurrentlyMoving && (currentElevator->CurrentRequest == passenger->CurrentRequest)) {
							// Check if the elevator we're currently checking is handling the process of this passenger
							// and not moving
							//printf("[Passenger %05d]  Enters elevator\r\n\r\n", GetCurrentThreadId());

							bool success = false;
							do {
								success = currentElevator->PassengerListMutex->try_lock();
							} while (!success);
							currentElevator->PassengerList->push_back(passenger);
							currentElevator->PassengerListMutex->unlock();

							passenger->InElevator = *it;
							break;
						}
					}
					break;
				default:
					//printf("Wait error: (%d)\r\n", GetLastError());
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
					success = passenger->InElevator->PassengerListMutex->try_lock();
				} while (!success);
				passenger->InElevator->PassengerList->remove(passenger);
				passenger->InElevator->PassengerListMutex->unlock();
				passenger->InElevator = nullptr;
				//printf("[Passenger %05d]  Exits elevator\r\n\r\n", GetCurrentThreadId());
			}
		}
	}

	// Wait for elevator to arrive at current floor
		
	
	return 0;
}

int Passenger::GetNextID() {
	int ID = Passenger::nextID;
	Passenger::nextID++;
	return ID;
}

Passenger::Passenger(const PassengerParameter& rhs)
	: ElevatableEntity(rhs)
	, InElevator(false)
{
	ID = GetNextID();
	CreateThread(0, 2048, Passenger::ThreadWork, (LPVOID)&Parameter, 0, &AssociatedThreadID);
}

Passenger::~Passenger()
{

}

int Passenger::SetFloor(const int floor) {
	Clear();

	ElevatableEntity::SetFloor(floor);

	Render();

	return floor;
}

void Passenger::Render() {
	COORD c;
	c.X = 10+ID;
	c.Y = Floor;
	ConsoleBufferHelper::JumpToPosition(c);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
	printf("P");
	ConsoleBufferHelper::GetMutex()->unlock();
}

void Passenger::Clear() {
	COORD c;
	c.X = 10+ID;
	c.Y = Floor;
	ConsoleBufferHelper::JumpToPosition(c);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
	printf(" ");
	ConsoleBufferHelper::GetMutex()->unlock();
}

void Passenger::SendRequest(const int floor)
{
	bool success = false;
	do {
		success = Parameter.OpenRequestsMutex->try_lock();
	} while (!success);
	
	//printf("[Passenger %05d]  Requesting elevator from %i to %i\r\n", GetCurrentThreadId(), Floor, floor);
	TargetFloor = floor;
	CurrentRequest = new FloorRequest(Floor, floor);
	Parameter.OpenRequests->push(CurrentRequest);
	Parameter.OpenRequestsMutex->unlock();
}

int Passenger::nextID = 0;