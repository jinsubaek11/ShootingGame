#pragma once

#include "CoreMinimal.h"
#include "Bullet.h"
#include "StrongBullet.generated.h"


UCLASS()
class SHOOTINGGAME_API AStrongBullet : public ABullet
{
	GENERATED_BODY()
	
public:
	AStrongBullet();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

};
