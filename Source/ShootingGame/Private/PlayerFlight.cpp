// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerFlight.h"
#include "components/BoxComponent.h"
#include "components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Bullet.h"

// Sets default values
APlayerFlight::APlayerFlight()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Collision 생성
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	// boxComp를 root로 설정
	SetRootComponent(boxComp);
	// boxComp 크기설정
	boxComp->SetBoxExtent(FVector(50));

	// Mesh 생성
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	// meshComp를 root아래로 설정
	meshComp->SetupAttachment(RootComponent);
	// static mesh에 넣을 mesh파일 로드
	ConstructorHelpers::FObjectFinder<UStaticMesh>cubemesh(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	// mesh파일 로드가 성공하면 세팅하라
	if (cubemesh.Succeeded())
	{
		meshComp->SetStaticMesh(cubemesh.Object);
	}
	// mesh location 조정
	meshComp->SetRelativeLocation(FVector(0, 0, -50));
}

// Called when the game starts or when spawned
void APlayerFlight::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerFlight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// 벡터 정규화
	direction.Normalize();
	// 이동 구현 p=p0+vt
	SetActorLocation(GetActorLocation() + direction * moveSpeed * DeltaTime);

}

// Called to bind functionality to input
void APlayerFlight::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Mapping 설정한 Horizontal input이 들어오면 HorizontalInput함수 실행
	PlayerInputComponent->BindAxis("Horizontal", this, &APlayerFlight::HorizontalInput);
	// Mapping 설정한 Vertical input이 들어오면 VerticalInput함수 실행
	PlayerInputComponent->BindAxis("Vertical", this, &APlayerFlight::VerticalInput);
	// Mapping 설정한 input이 들어오면 BulletFire함수 실행
	PlayerInputComponent->BindAction("Fire",IE_Pressed, this, &APlayerFlight::BulletFire);
}

// 좌우입력이 들어왔을 때 실행될 함수 정의
void APlayerFlight::HorizontalInput(float value)
{
	direction.Y = value;
}
// 상하입력이 들어왔을 때 실행될 함수 정의
void APlayerFlight::VerticalInput(float value)
{
	direction.Z = value;
}
// 클릭입력이 들어왔을 때 실행될 함수 정의
void APlayerFlight::BulletFire()
{
	// 스폰위치 정의
	FVector spawnPosition = GetActorLocation() + GetActorRightVector() * 200;
	// 스폰 로테이션 정의 ( Pitch, Roll, Yaw )
	FRotator spawnRotation = FRotator(0, 0, 0);
	// 스폰 옵션 (optional)
	FActorSpawnParameters para;
	para.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	// 총알 블루프린트를 넣은 변수를 스폰
	GetWorld()->SpawnActor<ABullet>(bulletFactory, spawnPosition, spawnRotation, para);
}