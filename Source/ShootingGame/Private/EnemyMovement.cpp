#include "EnemyMovement.h"

UEnemyMovement::UEnemyMovement()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UEnemyMovement::BeginPlay()
{
	Super::BeginPlay();
}

void UEnemyMovement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

int32 UEnemyMovement::GetCurrentTimeLineIndex()
{
	return timeLineIndex;
}

void UEnemyMovement::SetCurrentTimeLineIndex(int32 index)
{
	timeLineIndex = index;
}

FVector UEnemyMovement::CalcLinearInterpolate(const FVector& p1, const FVector& p2, float t)
{
	//UE_LOG(LogTemp, Warning, TEXT("%d %f"), timeLineIndex, t);
	return (1 - t) * p1 + t * p2;
}

FVector UEnemyMovement::Curve(const FVector& start, const FVector& p1, const FVector& p2, const FVector& dest, float t)
{
	//UE_LOG(LogTemp, Warning, TEXT("%d %f"), timeLineIndex, t);
	return (1 - t) * (1 - t) * (1 - t) * start +
		3 * t * (1 - t) * (1 - t) * p1 +
		3 * t * t * (1 - t) * p2 +
		t * t * t * dest;
}

void UEnemyMovement::Start(const FVector& start, const FVector& dest, float cumulativeTime, float duration)
{
	float t = cumulativeTime / duration;

	//FMath::Lerp()
	FVector position = FMath::Lerp<FVector, float>(start, dest, t);
	//FMath::Lerp<
	GetOwner()->SetActorLocation(position);
}

void UEnemyMovement::Custom(const TArray<FMovement>& path, const TArray<float>& timeLine, float duration, float cumulativeTime)
{
	float t = cumulativeTime / duration;

	if (t <= 1.f)
	{
		if (timeLineIndex == timeLine.Num() - 1) return;

		if (t >= timeLine[timeLineIndex + 1])
		{
			timeLineIndex++;
		}

		float segmentTime = t * path.Num() - timeLineIndex;

		switch (path[timeLineIndex].type)
		{
		case MovingType::LINEAR:
			GetOwner()->SetActorLocation(CalcLinearInterpolate(
				path[timeLineIndex].position, 
				path[timeLineIndex + 1].position, 
				segmentTime)
			);
			break;
		case MovingType::CURVE:
			GetOwner()->SetActorLocation(Curve(
				path[timeLineIndex].position,
				path[timeLineIndex].controlPoints[0],
				path[timeLineIndex].controlPoints[1],
				path[timeLineIndex + 1].position,
				segmentTime
			));
			break;
		default:
			break;
		}
	}

	return;
}

//bool UEnemyMovement::Straight(const FVector& destination, float duration, float cumulativeTime, bool isLinear, float speed, float acceleration)
//{
//	if (isStop) return true;
//
//	if (!isTransition)
//	{
//		startPosition = GetOwner()->GetActorLocation();
//		isTransition = true;
//	}
//
//	float t = cumulativeTime / duration;
//
//	if (t <= 1.f)
//	{
//		GetOwner()->SetActorLocation(CalcLinearInterpolate(startPosition, destination, t));
//		return true;
//	}
//
//	isTransition = false;
//	return false;
//}
//
//bool UEnemyMovement::RoundTripStraight(const FVector& returnPoint, float duration, float cumulativeTime, bool isLinear, float speed, float acceleration)
//{
//	if (isStop) return true;
//
//	if (!isTransition)
//	{
//		startPosition = GetOwner()->GetActorLocation();
//		isTransition = true;
//	}
//
//	float time = cumulativeTime / duration;
//
//	if (time * PI < PI)
//	{
//		float t = FMath::Sin(time * PI);
//		GetOwner()->SetActorLocation(CalcLinearInterpolate(startPosition, returnPoint, t));
//		return true;
//	}
//
//	return false;
//
//}
//
//bool UEnemyMovement::Triangle(const FVector& p1, const FVector& p2, float duration, float cumulativeTime)
//{
//	return true;
//
//}
//
//bool UEnemyMovement::Circle(const FVector& centerOffset, float duration, float cumulativeTime, float repeat)
//{
//	if (isStop) return true;
//
//	if (!isTransition)
//	{
//		startPosition = GetOwner()->GetActorLocation();
//		isTransition = true;
//	}
//	
//	float increaseTheta = cumulativeTime / duration * 2 * PI * repeat;
//
//	if (increaseTheta <= 2 * PI * repeat)
//	{
//		FVector rotateVec;
//		FVector vec = startPosition - centerOffset;
//		rotateVec.Y = vec.Y* FMath::Cos(increaseTheta) - vec.Z * FMath::Sin(increaseTheta);
//		rotateVec.Z = vec.Y* FMath::Sin(increaseTheta) + vec.Z * FMath::Cos(increaseTheta);
//
//		FVector position = rotateVec + centerOffset;
//
//		GetOwner()->SetActorLocation(position);
//		return true;
//	}
//
//	return false;
//}




