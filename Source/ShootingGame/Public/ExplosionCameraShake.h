#pragma once

#include "CoreMinimal.h"
#include "LegacyCameraShake.h"
#include "ExplosionCameraShake.generated.h"


UCLASS()
class SHOOTINGGAME_API UExplosionCameraShake : public ULegacyCameraShake
{
	GENERATED_BODY()
	
public:
	UExplosionCameraShake();
};
