#pragma once

#include "math/arrayEx.hpp"

class Camera {
private:
	SDL_FRect view;
	bool enabled;

	float zoom = 1.0f;
public:
	Camera(const Float2& camSize);
	Camera() = delete;
	virtual ~Camera();

	void setCamSize(const Float2 camSize);

	float getZoom() const;
	void setZoom(const float zoom);

	void enable(const bool use);

	SDL_FRect worldToView(const SDL_FRect& worldRect) const;
	Float2 worldToView(const Float2& worldPos) const;

	SDL_FPoint viewToWorld(const SDL_FPoint& viewPoint) const;

	void setPosition(const Float2& pos);
	void follow(const Float2& targetPos);

	const SDL_FRect& getView() const;
	bool getEnabled() const;
	const Float2 getCamSize() const;
	const Float2 getPos() const;
};