#include "CStateComponent.h"

UCStateComponent::UCStateComponent()
{
}


void UCStateComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UCStateComponent::SetIdleMode()
{
	ChangeType(EStateType::Idle);
}

void UCStateComponent::SetRollMode()
{
	ChangeType(EStateType::Roll);
}

void UCStateComponent::SetBackstepMode()
{
	ChangeType(EStateType::Backstep);
}

void UCStateComponent::SetEquipMode()
{
	ChangeType(EStateType::Equip);
}

void UCStateComponent::SetUnequipMode()
{
	ChangeType(EStateType::Unequip);
}

void UCStateComponent::ChangeType(EStateType InNewType)
{
	EStateType PrevType = Type;
	Type = InNewType;

	OnStateTypeChanged.Broadcast(PrevType, Type);
}
