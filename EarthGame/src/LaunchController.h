#pragma once

#include "../GamesEngineeringBase.h"
#include "Vector2.h"
#include "Scene.h"
#include "BasePhysicalObject.h"

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

	float minDragDistance = 10.f;
	float maxDragDistance = 200.0f;
	float launchStrength = 1000.0f;

	bool leftMouseDown = false;

	Vector2 launchPoint;
	Vector2 mouseDirection;

	GamesEngineeringBase::Window& canvas;
	BasePhysicalObject* earth;

	void Release();
public:
	LaunchController(GamesEngineeringBase::Window& win,
		BasePhysicalObject* target);

	void UpdateInput();

	void UpdateAiming();

	LaunchState GetState() const { return state; }

	Vector2 GetLaunchDir() {
		if (state == LaunchState::Aiming && mouseDirection.Length() > 1e-4f) {
			return mouseDirection / mouseDirection.Length();
		}
		else {
			return Vector2();
		}
	}
};

