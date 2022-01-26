#include "ObjComponent.h"

ObjComponent::ObjComponent(std::string file, Shader shader) : file(file), shader(shader) {}

ObjComponent::~ObjComponent() {
	for (RenderObject *obj : parts) {
		std::vector<RenderObject*>::iterator it = std::find(gameObject->renderObjects.begin(), gameObject->renderObjects.end(), obj);
		gameObject->renderObjects.erase(it);
		delete(obj);
	}
}

void ObjComponent::onAdded(PiE::EngineContext & ctx, GameObject & gameObject) {
	std::vector<RenderObject> ROs = OBJ::parseObj(file.c_str());
	for (RenderObject obj : ROs) {
		obj.renderContext.shader = &shader;
		RenderObject *objptr = new RenderObject(obj);
		parts.push_back(objptr);
		gameObject.renderObjects.push_back(objptr);
		if (objptr != parts[0]) {
			objptr->transform.parent = &parts[0]->transform;
		}
		else {
			objptr->transform.parent = &gameObject.transform;
		}
	}
}
