#pragma once

#include "../GamesEngineeringBase.h"
#include "Vector2.h"
#include "Scene.h"
#include "BasePhysicalObject.h"
#include "planets/BasePlanet.h"

class Camera;
class TestObject;

enum class LaunchState {
	Ready,
	Aiming,
	Flying
};

class LaunchController {
private:
	LaunchState state = LaunchState::Ready;
	bool leftMouseDown = false;

	Vector2 launchPoint;
	Vector2 mouseDirection;
	Vector2 startScale;

	GamesEngineeringBase::Window& canvas;
	BasePlanet* earth;

	void Release();
public:
	float minDragDistance = 10.f;
	float maxDragDistance = 400.0f;
	float launchStrength;

	LaunchController(GamesEngineeringBase::Window& win,
		BasePlanet* target);

	void UpdateInput();

	void UpdateAiming();

	LaunchState GetState() const { return state; }

	Vector2 GetLaunchDrag() {
		if (state == LaunchState::Aiming && mouseDirection.Length() > 1e-4f) {
			return mouseDirection;
		}
		else {
			return Vector2();
		}
	}
};

