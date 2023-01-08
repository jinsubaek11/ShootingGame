#include "ExplosionCameraShake.h"


UExplosionCameraShake::UExplosionCameraShake()
{
	OscillationDuration = 0.25f;
	OscillationBlendInTime = 0.05f;
	OscillationBlendOutTime = 0.05f;

	RotOscillation.Pitch.Amplitude = 0.5f;
	RotOscillation.Pitch.Frequency = 0.5f;

	RotOscillation.Yaw.Amplitude = 0.5f;
	RotOscillation.Yaw.Frequency = 0.5f;
}
