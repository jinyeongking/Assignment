#include "CDoor.h"
#include "Global.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Characters/CPlayer.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/GameplayStatics.h"

ACDoor::ACDoor()
{
    // Root Component Setup
    RootSceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComp"));
    RootComponent = RootSceneComp;

    // Collision Component Setup
    CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
    CollisionComp->SetupAttachment(RootComponent);
    CollisionComp->SetRelativeScale3D(FVector(3));
    CollisionComp->bHiddenInGame = false;

    // 메시 구성 요소 설정
    FrameMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FrameMesh"));
    FrameMesh->SetupAttachment(RootComponent);
    DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
    DoorMesh->SetupAttachment(FrameMesh);

    // 정적 메시 로드 및 할당
    UStaticMesh* LoadedFrameMesh;
    UStaticMesh* LoadedDoorMesh;
    CHelpers::GetAsset(&LoadedFrameMesh, TEXT("/Game/Assignment/Door/Props/SM_DoorFrame"));
    CHelpers::GetAsset(&LoadedDoorMesh, TEXT("/Game/Assignment/Door/Props/SM_Door"));
    if (LoadedFrameMesh)
    {
        FrameMesh->SetStaticMesh(LoadedFrameMesh);
        FrameMesh->SetRelativeLocation(FVector(0, 0, -100));
    }
    if (LoadedDoorMesh)
    {
        DoorMesh->SetStaticMesh(LoadedDoorMesh);
        DoorMesh->SetRelativeLocation(FVector(0, 45, 0));
    }

    bPrimaryAction = false;
    //기본 키 유형을 설정하고 편집기에서 변경할 수 있습니다
    RequiredKey = EPrimaryAction::Red; 
}

 void ACDoor::OnConstruction(const FTransform& Transform)
 {
     if (!FrameMaterialInstance || !DoorMaterialInstance)
     {
     FrameMaterialInstance = UMaterialInstanceDynamic::Create(FrameMesh->GetMaterial(0), this);
     DoorMaterialInstance = UMaterialInstanceDynamic::Create(DoorMesh->GetMaterial(0), this);
     if (FrameMaterialInstance)
     {
        FrameMesh->SetMaterial(0, FrameMaterialInstance);
     }
     if (DoorMaterialInstance)
     {
        DoorMesh->SetMaterial(0, DoorMaterialInstance);
     }
}

if (FrameMaterialInstance)
{
    FrameMaterialInstance->SetVectorParameterValue(TEXT("Color"), MaterialColor);
}
if (DoorMaterialInstance)
{
    DoorMaterialInstance->SetVectorParameterValue(TEXT("ColorDoor"), MaterialColor);
}
}

void ACDoor::BeginPlay()
{
    Super::BeginPlay();
    CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ACDoor::BeginOverlap);
    CollisionComp->OnComponentEndOverlap.AddDynamic(this, &ACDoor::EndOverlap);
}

EPrimaryAction ACDoor::OnPrimaryAction()
{
    if (bPrimaryAction)
    {
        UE_LOG(LogTemp, Warning, TEXT("Door has already been opened."));
        return EPrimaryAction::None;
    }

    //플레이어가 올바른 키를 가지고 있는지 확인합니다
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PlayerController)
    {
        ACPlayer* Player = Cast<ACPlayer>(PlayerController->GetPawn());
        if (Player && Player->HasKey(RequiredKey))
        {
            // 문을 연 상태로 표시
            bPrimaryAction = true;
            UE_LOG(LogTemp, Warning, TEXT("Door opened!"));
            Open();  // 오프닝 동작 트리거
            return EPrimaryAction::None;  // 문을 열 때 키가 주어지지 않게함
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("You don't have the correct key to open this door!"));
            return EPrimaryAction::None;
        }
    }
    return EPrimaryAction::None;
}

void ACDoor::FailInteract() {
    UE_LOG(LogTemp, Warning, TEXT("You don't have the correct key to open this door!"));
}

EPrimaryAction ACDoor::GetType() const {
    return RequiredKey;
}

void ACDoor::Open_Implementation() {
    FRotator OpenRotation = FRotator(0, -90, 0);
    if (DoorMesh)
    {
        UE_LOG(LogTemp, Warning, TEXT("Door is opening."));
        DoorMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);  
        DoorMesh->AddRelativeRotation(OpenRotation);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("DoorMesh is null! Initialization failed."));
    }
}

void ACDoor::PerformOpenLerp() {
    CurrentRotation = FMath::Lerp(CurrentRotation, TargetRotation, 0.15f);
    if (DoorMesh)
    {
        DoorMesh->SetRelativeRotation(CurrentRotation);
    }
    if (FMath::IsNearlyEqual(CurrentRotation.Yaw, TargetRotation.Yaw, 0.1f))
    {
        GetWorld()->GetTimerManager().ClearTimer(OpenTimerHandle);
    }
}

void ACDoor::SetInteracted() {
    bPrimaryAction = true;
}

void ACDoor::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
    FString Message = "Press F to Open. You must have a ";
    switch (RequiredKey)
    {
    case EPrimaryAction::Red:
        Message.Append(TEXT("Red Key."));
        break;
    case EPrimaryAction::Blue:
        Message.Append(TEXT("Blue Key."));
        break;
    case EPrimaryAction::Green:
        Message.Append(TEXT("Green Key."));
        break;
    }
    DrawDebugString(GetWorld(), GetActorLocation(), Message, nullptr, FColor::White, 0.8f);
}

void ACDoor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) 
{
  
}