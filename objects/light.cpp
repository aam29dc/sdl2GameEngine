#include "objects/light.hpp"
#include "objects/SDLGameObject.hpp"
#include "core/renderer.hpp"
#include "core/camera.hpp"
#include <cmath>

Light::Light(const Float2& pos, const float z, const float intensity) : PhysicsObject(pos, { 0,0 }, { 0,0 }) {
	this->pos = pos;
	this->z = z;
	this->intensity = intensity;
}

void Light::setZ(const float z) {
	this->z = z;
}

float Light::getZ() const {
	return z;
}

void Light::drawShadows(Renderer* renderer, const std::vector<SDLGameObject*>& objects, const Camera& camera, const bool& iso) {
	for (const auto& obj : objects) {
		float dz = 1.0f - z;
		float dx = 0.0f;

		SDL_FRect shadow = {};

		// Light is to the left of the object
		if (obj->getPos().x >= pos.x) {
			dx = (obj->getPos().x + obj->getSize().x) - pos.x;
			float baseX = (dz - 1.0f) * dx / dz;
			float wX = std::min(std::fabs(baseX - dx), static_cast<float>(SDLGameObject::SIZE));

			shadow = {
				obj->getPos().x + obj->getSize().x,
				obj->getPos().y,
				wX,
				obj->getSize().y
			};
		}
		// Light is to the right of the object
		else {
			dx = obj->getPos().x - pos.x;
			float baseX = (dz - 1.0f) * dx / dz;
			float wX = std::min(std::fabs(baseX - dx), static_cast<float>(SDLGameObject::SIZE));

			shadow = {
				obj->getPos().x - wX,
				obj->getPos().y,
				wX,
				obj->getSize().y
			};
		}

		shadow = camera.worldToView(shadow);
		if (iso) shadow = renderer->viewToIso(shadow);
		shadow = renderer->scale(shadow);
		SDL_SetRenderDrawColor(renderer->sdlRenderer, 0, 0, 0, (Uint8)(60/intensity));
		SDL_RenderFillRectF(renderer->sdlRenderer, &shadow);
	}
}