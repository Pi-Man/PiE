#pragma once
#include <PiE.h>
//#include <Vec.h>
//#include <PerlinNoise2D.h>
struct RandomPositionRotation : Component {

	float time = 0;

	Vec3f position{};
	Vec3f rotation{};

	PerlinNoise2D noise;

	RandomPositionRotation(int seed) : noise(seed) {}

	// called once every logic tick
	virtual void fixedUpdate(PiE::EngineContext &ctx);

	// called once the game object this component is attached to is successfully added
	virtual void onAdded(PiE::EngineContext &ctx, GameObject &gameObject);

};

