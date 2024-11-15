#include "CChest.h"
#include "Global.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Characters/CPlayer.h"
#include "Materials/MaterialInstanceDynamic.h"
ACChest::ACChest()
{
    // Root Component
    CHelpers::CreateSceneComponent(this, &RootSceneComp, "RootSceneComp");
    RootComponent = RootSceneComp;

    // Collision Component
    CHelpers::CreateSceneComponent(this, &BoxComp, "BoxComp", RootComponent);
    BoxComp->SetRelativeScale3D(FVector(3));
    BoxComp->bHiddenInGame = false;

    // Mesh Components
    CHelpers::CreateSceneComponent(this, &DownMeshComp, "DownMeshComp", RootComponent);
    CHelpers::CreateSceneComponent(this, &UpMeshComp, "UpMeshComp", DownMeshComp);

    UStaticMesh* UpMeshAsset;
    UStaticMesh* DownMeshAsset;
    CHelpers::GetAsset(&UpMeshAsset, "/Game/Assignment/Chest/SM_ChestTop");
    CHelpers::GetAsset(&DownMeshAsset, "/Game/Assignment/Chest/SM_ChestBottom");

    if (UpMeshAsset)
    {
        UpMeshComp->SetStaticMesh(UpMeshAsset);
    }
    if (DownMeshAsset)
    {
        DownMeshComp->SetStaticMesh(DownMeshAsset);
    }
    DownMeshComp->SetRelativeLocation(FVector(-40, 0, -30));

    // �ʱ� ����
    bPrimaryAction = false;

    //�⺻ Ű ������ �����ϰ� ���� ���迡 ���� ����
    Type = EPrimaryAction::Red;  
}

void ACChest::OnConstruction(const FTransform& Transform) 
{
    Super::OnConstruction(Transform);

    if (!DownMaterial || !UpMaterial)
    {
        DownMaterial = UMaterialInstanceDynamic::Create(DownMeshComp->GetMaterial(0), nullptr);
        UpMaterial = UMaterialInstanceDynamic::Create(UpMeshComp->GetMaterial(0), nullptr);
        if (DownMaterial)
        {
            DownMeshComp->SetMaterial(0, DownMaterial);
        }
        if (UpMaterial)
        {
            UpMeshComp->SetMaterial(0, UpMaterial);
        }
    }

    if (DownMaterial)
    {
        DownMaterial->SetVectorParameterValue("Emissive", Emissive);
    }
    if (UpMaterial)
    {
        UpMaterial->SetVectorParameterValue("Emissive", Emissive);
    }
}

void ACChest::BeginPlay() 
{
    Super::BeginPlay();
    BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ACChest::BeginOverlap);
    BoxComp->OnComponentEndOverlap.AddDynamic(this, &ACChest::EndOverlap);
}

EPrimaryAction ACChest::OnPrimaryAction() 
{
    if (bPrimaryAction)
    {
        UE_LOG(LogTemp, Warning, TEXT("Chest has already been interacted with."));
        return EPrimaryAction::None;
    }

    // ���ڸ� ��ȣ �ۿ��Ѱ����� ǥ��.
    bPrimaryAction = true;

    // Ʈ���� on
    Open_Implementation();
    switch (Type)
    {
    case EPrimaryAction::Red:
        UE_LOG(LogTemp, Warning, TEXT("Player collected a Red key!"));
        break;
    case EPrimaryAction::Green:
        UE_LOG(LogTemp, Warning, TEXT("Player collected a Green key!"));
        break;
    case EPrimaryAction::Blue:
        UE_LOG(LogTemp, Warning, TEXT("Player collected a Blue key!"));
        break;
    default:
        UE_LOG(LogTemp, Warning, TEXT("Player collected an unknown key!"));
        break;
    }

    //�� ���ڿ��� �����ϴ� Ű ���� ��ȯ
    return Type;
}

void ACChest::FailInteract() 
{
    UE_LOG(LogTemp, Warning, TEXT("Interaction failed!"));
}

EPrimaryAction ACChest::GetType() const 
{
    return Type;
}

void ACChest::SetInteracted() 
{
    bPrimaryAction = true;
}

void ACChest::Open_Implementation()
{
    UpMeshComp->AddRelativeRotation(FRotator(110, 0, 0));
    UpMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACChest::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) 
{
    FString msg = "Press F to Interact!";
    DrawDebugString(GetWorld(), GetActorLocation(), msg, nullptr, FColor::White, 2.0f);
}

void ACChest::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
   
}
