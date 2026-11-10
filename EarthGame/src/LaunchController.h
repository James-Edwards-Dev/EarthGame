#pragma once

#include"../GamesEngineeringBase.h"
#include"Vector2.h"
#include"Scene.h"

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

	float maxDragDistance = 200.0f;
	float launchStrength = 200.0f;

	bool leftMouseDown = false;
	bool wasReset = false;

	Vector2 launchPoint;
	Vector2 mouseDirection;

	GamesEngineeringBase::Window& canvas;
	Camera& camera;
	TestObject& earth;

	void Release();
public:
	LaunchController(GamesEngineeringBase::Window& win,
		Camera& cam,
		TestObject& target);

	void UpdateInput();

	void UpdateAiming();

	void Reset();

	LaunchState GetState() const;
};

