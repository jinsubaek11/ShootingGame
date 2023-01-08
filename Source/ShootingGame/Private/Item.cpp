// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "components/BoxComponent.h"
#include "components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "PlayerFlight.h"
#include "Fence_Vertical.h"
#include "Fence_Horizontal.h"
#include "TengaiGameMode.h"
#include "ItemWidget.h"
#include "PaperFlipbookComponent.h"


// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(25));
	boxComp->SetCollisionProfileName(TEXT("ItemPreset"));

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	meshComp->SetupAttachment(RootComponent);
	
	widgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget Component"));
	widgetComp->SetupAttachment(RootComponent);
	widgetComp->SetRelativeRotation(FRotator(0, -90, 0));
	widgetComp->SetPivot(FVector2D(-0.1, 0.15));
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnOverlap);

	randomDir.Y = FMath::RandRange(-100.0f, 100.0f);
	randomDir.Z = FMath::RandRange(-100.0f, 100.0f);
	randomDir.Normalize();

	UItemWidget* itemWidget = Cast<UItemWidget>(widgetComp->GetWidget());
	if (itemWidget)
	{
		itemWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isTouched)
	{
		randomDir = FVector(0, 0, 1);
	}

	SetActorLocation(GetActorLocation() + randomDir * itemSpeed * DeltaTime, true);

}

void AItem::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerFlight* player = Cast<APlayerFlight>(OtherActor);
	if (player != nullptr && !player->GetIsDead())
	{
		ItemSelector(player);

		SetActorEnableCollision(false);

		UItemWidget* itemWidget = Cast<UItemWidget>(widgetComp->GetWidget());
		if (itemWidget)
		{
			itemWidget->SetVisibility(ESlateVisibility::Visible);
			itemWidget->PrintGetItemInfo(GetItemType());
			isTouched = true;
		}

		ATengaiGameMode* tengaiGM = Cast<ATengaiGameMode>(GetWorld()->GetAuthGameMode());
		if (tengaiGM)
		{
			tengaiGM->AddScore(point);
		}

		GetWorldTimerManager().SetTimer(timer, this, &AItem::DestroySelf, 0.8, false);
	}

	AFence_Vertical* fenceVer = Cast<AFence_Vertical>(OtherActor);
	if (fenceVer != nullptr)
	{
		randomDir.Y *= -1;
		//UE_LOG(LogTemp, Warning, TEXT("toched v"));
		return;
	}

	AFence_Horizontal* fenceHor = Cast<AFence_Horizontal>(OtherActor);
	if (fenceHor != nullptr)
	{
		randomDir.Z *= -1;
		//UE_LOG(LogTemp, Warning, TEXT("toched h"));
	}
}

void AItem::ItemSelector(APlayerFlight* player)
{
}

ItemType AItem::GetItemType() const
{
	return type;
}

void AItem::DestroySelf()
{
	Destroy();
}
