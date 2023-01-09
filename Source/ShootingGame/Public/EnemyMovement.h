#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyMovement.generated.h"

UENUM()
enum class MovingType : uint8
{
	LINEAR,
	CURVE,
	NONE,
	//CIRCLE,
};

USTRUCT()
struct FMovement
{
	GENERATED_BODY()

	UPROPERTY()
	MovingType type;
	UPROPERTY()
	FVector position;
	UPROPERTY()
	FVector controlPoints[2];
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTINGGAME_API UEnemyMovement : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEnemyMovement();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	int32 GetCurrentTimeLineIndex();
	void SetCurrentTimeLineIndex(int32 index);

	FVector CalcLinearInterpolate(const FVector& p1, const FVector& p2, float t);
	FVector Curve(const FVector& start, const FVector& p1, const FVector& p2, const FVector& dest, float t);
	void Start(const FVector& start, const FVector& dest, float cumulativeTime, float duration);
	void Custom(const TArray<FMovement>& path, const TArray<float>& timeLine, float duration, float cumulativeTime);
	
private:
	uint16 timeLineIndex;
};


//bool Straight(
//	const FVector& destination, float duration, float cumulativeTime, 
//	bool isLinear = true, float speed = 1.f, float acceleration = 0.01f
//);
//bool RoundTripStraight(const FVector& returnPoint, float duration, float cumulativeTime,
//	bool isLinear = true, float speed = 1.f, float acceleration = 0.01f
//);
//bool Triangle(const FVector& p1, const FVector& p2, float duration, float cumulativeTime);
//bool Circle(const FVector& centerOffset, float duration, float cumulativeTime, float repeat = 1);
