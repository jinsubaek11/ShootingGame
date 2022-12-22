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

	// Collision ����
	boxcomp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	// boxcomp�� root�� ����
	SetRootComponent(boxcomp);
	// boxcomp ũ�⼳��
	boxcomp->SetBoxExtent(FVector(50));

	// Mesh ����
	meshcomp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	// meshcomp�� root�Ʒ��� ����
	meshcomp->SetupAttachment(RootComponent);
	// static mesh�� ���� mesh���� �ε�
	ConstructorHelpers::FObjectFinder<UStaticMesh>cubemesh(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	// mesh���� �ε尡 �����ϸ� �����϶�
	if (cubemesh.Succeeded())
	{
		meshcomp->SetStaticMesh(cubemesh.Object);
	}
	// mesh location ����
	meshcomp->SetRelativeLocation(FVector(0, 0, -50));
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
	// ���� ����ȭ
	direction.Normalize();
	// �̵� ���� p=p0+vt
	SetActorLocation(GetActorLocation() + direction * movespeed * DeltaTime);

}

// Called to bind functionality to input
void APlayerFlight::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Mapping ������ Horizontal input�� ������ horizontalinput�Լ� ����
	PlayerInputComponent->BindAxis("Horizontal", this, &APlayerFlight::horizontalinput);
	// Mapping ������ Vertical input�� ������ verticalinput�Լ� ����
	PlayerInputComponent->BindAxis("Vertical", this, &APlayerFlight::verticalinput);
	// Mapping ������ input�� ������ bulletfire�Լ� ����
	PlayerInputComponent->BindAction("Fire",IE_Pressed, this, &APlayerFlight::bulletfire);
}

// �¿��Է��� ������ �� ����� �Լ� ����
void APlayerFlight::horizontalinput(float value)
{
	direction.Y = value;
}
// �����Է��� ������ �� ����� �Լ� ����
void APlayerFlight::verticalinput(float value)
{
	direction.Z = value;
}
// Ŭ���Է��� ������ �� ����� �Լ� ����
void APlayerFlight::bulletfire()
{
	// ������ġ ����
	FVector spawnposition = GetActorLocation() + GetActorRightVector() * 60;
	// ���� �����̼� ���� ( Pitch, Roll, Yaw )
	FRotator spawnrotation = FRotator(0, 0, 0);
	// ���� �ɼ� (optional)
	FActorSpawnParameters para;
	para.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	// �Ѿ� ��������Ʈ�� ���� ������ ����
	GetWorld()->SpawnActor<ABullet>(bulletfactory, spawnposition, spawnrotation, para);
}