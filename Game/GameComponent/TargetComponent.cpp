#include "TargetComponent.h"

void TargetComponent::Update()
{
}

bool TargetComponent::Write(const rapidjson::Value& value) const
{
	return false;
}

bool TargetComponent::Read(const rapidjson::Value& value)
{
	return true;
}
