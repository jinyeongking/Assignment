#include "CPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Global.h"

void ACPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("OptionMenu", IE_Pressed, this, &ACPlayerController::ToggleOptionMenu);

}

void ACPlayerController::ToggleOptionMenu()
{
	CheckNull(OptionMenuClass);

	if (OptionMenu && OptionMenu->IsInViewport())
	{
		OptionMenu->RemoveFromParent();
		OptionMenu = nullptr;

		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());
		return;
	}

	OptionMenu = CreateWidget<UUserWidget>(this, OptionMenuClass);
	CheckNull(OptionMenu);

	OptionMenu->AddToViewport(100);

	bShowMouseCursor = true;
	SetInputMode(FInputModeGameAndUI());

	// 1. 실제 WB 메뉴가 없음
	// 2. GM에 PC를 등록해야 한다
}
