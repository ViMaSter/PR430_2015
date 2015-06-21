#include "stdafx.h"

#include "ElevatableEntity.h"

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
