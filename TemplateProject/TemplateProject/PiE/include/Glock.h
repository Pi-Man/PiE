#ifndef _GLOCK
#define _GLOCK

#include "PiE.h"

class GlockCMP : public Component {
	Uint32 animStart;
	bool animPlaying;
	ObjComponent *obj;
	PointLight flash;
public:
	GlockCMP(ObjComponent *obj);
	void fixedUpdate(PiE::EngineContext &ctx) override;
	void onAdded(PiE::EngineContext &ctx, GameObject &gameObject) override;
};



struct Glock : public GameObject {
	GlockCMP *glockCMP;
	ObjComponent *obj;
	Glock(Shader shader);
	~Glock();
};



#endif