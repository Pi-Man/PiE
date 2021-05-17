#ifndef _TRIANGLE_COMPONENT
#define _TRIANGLE_COMPONENT

#include "PiE.h"

class TriangleComponent : public Component {
	
	const std::array<float, 9> points;

	int tick = 0;

public:

	TriangleComponent(std::array<float, 9> points);
	~TriangleComponent();

	void fixedUpdate(PiE::EngineContext &ctx) override;
	void onAdded(PiE::EngineContext &ctx, GameObject &gameObject) override;

};

#endif