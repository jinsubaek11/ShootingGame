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


// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// �ڽ� �ݸ��� ����
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(25));
	boxComp->SetCollisionProfileName(TEXT("ItemPreset"));

	// �޽� ����
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	meshComp->SetupAttachment(RootComponent);

	
	//widgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget Component"));
	//widgetComp->SetWidget(CreateWidget<UItemWidget>(GetWorld(), itemWidgetClass));
	//widgetComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnOverlap);

	randomDir.Y = FMath::RandRange(-100.0f, 100.0f);
	randomDir.Z = FMath::RandRange(-100.0f, 100.0f);
	randomDir.Normalize();

}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ������ ������ ������ �������� �̵��ϰ�
	SetActorLocation(GetActorLocation() + randomDir * itemSpeed * DeltaTime, true);

}

void AItem::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerFlight* player = Cast<APlayerFlight>(OtherActor);
	if (player != nullptr && !player->GetIsDead())
	{
		ItemSelector(player);
		
		//ATengaiGameMode* gm = Cast<ATengaiGameMode>(GetWorld()->GetAuthGameMode());
		//gm->itemUI->PrintGetItemInfo(GetItemType(), GetActorLocation());

		//UItemWidget* itemWidget = Cast<UItemWidget>(widgetComp->GetWidget());
		//if (itemWidget)
		//{
		//	itemWidget->PrintGetItemInfo(GetItemType(), GetActorLocation());
		//}


		Destroy();
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
