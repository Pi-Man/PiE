#include "RandomPositionRotation.h"

void RandomPositionRotation::fixedUpdate(PiE::EngineContext & ctx) {

	float deltaTime = 1.0f / ctx.tickLimiter.getStaticUPS();

	time += deltaTime;

	position[0] += noise.get(0, time * 1.0/1.0) * 20 * deltaTime;
	position[1] += noise.get(1, time * 6.0/7.0) * 20 * deltaTime;
	position[2] += noise.get(2, time * 4.0/5.0) * 20 * deltaTime;

	rotation[0] += noise.get(3, time * 02.0/03.0) * 6 * deltaTime;
	rotation[1] += noise.get(4, time * 10.0/11.0) * 6 * deltaTime;
	rotation[2] += noise.get(5, time * 12.0/13.0) * 6 * deltaTime;

	gameObject->transform.m = Matrix4f::Identity().rotateR<AXIS::Y, AXIS::Z>(rotation[0]).rotateR<AXIS::Z, AXIS::X>(rotation[1]).rotateR<AXIS::X, AXIS::Y>(rotation[2]).translate(position);
}

void RandomPositionRotation::onAdded(PiE::EngineContext & ctx, GameObject & gameObject) {
	position = Vec3f{ {(float)noise.get(0.5, 0.5), (float)noise.get(2.5, 2.5), (float)noise.get(4.5, 4.5)} } * 20; // random initial position
	rotation = { {0, 0, 0} };
	time = 0;
}
