#include "ObjComponent.h"

namespace PiE {

	ObjComponent::ObjComponent(std::string file, Shader shader, VertexFormat format) : file(file), shader(shader), format(format) {}

	ObjComponent::~ObjComponent() {
		for (RenderObject * obj : parts) {
			std::vector<RenderObject *>::iterator it = std::find(gameObject->renderObjects.begin(), gameObject->renderObjects.end(), obj);
			gameObject->renderObjects.erase(it);
			delete(obj);
		}
	}

	void ObjComponent::onAdded(PiE::EngineContext & ctx, GameObject & gameObject) {
		std::vector<RenderObject> ROs = OBJ::parseObj(file.c_str(), format);
		for (RenderObject obj : ROs) {
			obj.renderContext.shader = &shader;
			RenderObject * objptr = new RenderObject(obj);
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

}