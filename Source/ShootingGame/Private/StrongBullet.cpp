#include "StrongBullet.h"
#include "components/BoxComponent.h"
#include "components/StaticMeshComponent.h"


AStrongBullet::AStrongBullet()
{
	boxComp->SetBoxExtent(FVector(50));
	SetRootComponent(boxComp);

	ConstructorHelpers::FObjectFinder<UStaticMesh> sphereMesh(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	meshComp->SetStaticMesh(sphereMesh.Object);
	meshComp->SetWorldScale3D(FVector(0.5));
	meshComp->SetRelativeLocation(FVector(0, 0, -10));
}

void AStrongBullet::BeginPlay()
{
	Super::BeginPlay();
}

void AStrongBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
