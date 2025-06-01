#include "camera.hpp"

Camera::Camera(const Float2& camSize) {
	enabled = true;
	view = { 0, 0, camSize.x, camSize.y };
	zoom = 1.0f;
}

Camera::~Camera() {

}

void Camera::setCamSize(const Float2 camSize) {
	view.w = camSize.x;
	view.h = camSize.y;
}

float Camera::getZoom() const {
	return zoom;
}

void Camera::setZoom(const float zoom) {
	this->zoom = zoom;
}

void Camera::enable(const bool use) {
	enabled = use;
}

SDL_FRect Camera::worldToView(const SDL_FRect& worldRect) const {
	if (!enabled) return worldRect;

	return {
		(worldRect.x - view.x) * zoom,
		(worldRect.y - view.y) * zoom,
		worldRect.w * zoom,
		worldRect.h * zoom
	};
}

Float2 Camera::worldToView(const Float2& worldPos) const {
	if (!enabled) return worldPos;

	return {
		(worldPos.x - view.x) * zoom,
		(worldPos.y - view.y) * zoom,
	};
}

SDL_FPoint Camera::viewToWorld(const SDL_FPoint& viewPoint) const {
	if (!enabled) return viewPoint;

	return {
		(viewPoint.x + view.x)/zoom,
		(viewPoint.y + view.y)/zoom
	};
}

void Camera::setPosition(const Float2& pos) {
	view.x = pos.x;
	view.y = pos.y;
}

void Camera::follow(const Float2& targetPos) {
	view.x = targetPos.x - (view.w / (2.0f * zoom));
	view.y = targetPos.y - (view.h / (2.0f * zoom));
}

const SDL_FRect& Camera::getView() const {
	return view;
}

bool Camera::getEnabled() const {
	return enabled;
}

const Float2 Camera::getCamSize() const {
	return { view.w, view.h };
}

const Float2 Camera::getPos() const {
	return { (view.x * 2.0f / zoom) + view.w, (view.y * 2.0f / zoom) + view.h };
}