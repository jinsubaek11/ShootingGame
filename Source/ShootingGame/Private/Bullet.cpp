// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "components/BoxComponent.h"
#include "components/MeshComponent.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// collision 생성
	boxcomp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	// boxcomp를 root로 설정
	SetRootComponent(boxcomp);
	// collision 크기 설정
	boxcomp->SetBoxExtent(FVector(50));
	// collision 스케일 설정
	boxcomp->SetWorldScale3D(FVector(0.75, 1, 0.25));

	// mesh 생성
	meshcomp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	// meshcomp를 root아래로 설정
	meshcomp->SetupAttachment(RootComponent);
	// mesh 위치조정
	meshcomp->SetRelativeLocation(FVector(0, 0, -50));
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 액터기준 정면(우측) 벡터값 받아오기
	direction = GetActorRightVector();
	// 이동구현 p=p0+vt
	SetActorLocation(GetActorLocation() + direction * bulletspeed * DeltaTime);



}

