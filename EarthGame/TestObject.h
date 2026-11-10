#pragma once
#include "src/BasePhysicalObject.h"
#include "GamesEngineeringBase.h"
#include "src/Sprite.h"
#include "src/LaunchController.h"
#include "src/MyMath.h"

class TestObject : public BasePhysicalObject {
private:
	LaunchController launchController;
	
	float waitingTime = 0.0f;
	float maxWaitingTime = 30.0f;

	float solarSystemRadius = 700.0f;

	float minFOV = 360;
	float maxFOV = 1920;

public:
	float maxForce;
	Sprite sprite;

	TestObject(GamesEngineeringBase::Window& canvas) :
		BasePhysicalObject(canvas){
		scale = { 100,100 };
		rotation = 0;
		mass = 300;
		maxForce = 400 * mass; // global position per second
		maxVelocity = 1000;
		renderOrder = RenderOrder::Third;
		prevState = { globalPosition, scale, rotation };
		curState = { globalPosition, scale, rotation };
		canCollide = true;
		collisionRadius = 40;
		isGravitySource = true;
	}

	void OnStart() override {
		sprite.Load("./assets/planet00.png");
	}

	void FixedUpdate(const float& dt) override {
		force = scene->GetGravityOn(this);
		BasePhysicalObject::UpdatePhysicPosition(dt);
	}

	void VisualUpdate(const float& dt) override {
		launchController.UpdateInput();
		if (launchController.GetState() != LaunchState::Flying) {
			waitingTime += dt;
		}
		if (waitingTime >= maxWaitingTime) {
			scene->ChangeScene(SceneType::HesitateEnd);
		}
		if (launchController.GetState() == LaunchState::Flying) {
			Vector2 difference = globalPosition - scene->GetSunPosition();
			float distanceToSun = difference.Length();
			if (distanceToSun > solarSystemRadius) {
				scene->ChangeScene(SceneType::TheEyeEnd);
			}
			float alpha = std::clamp(velocity.Length(), 0.f, maxVelocity) / maxVelocity;
			FOV = MyMath::Lerp(FOV, minFOV * (1 - alpha) + maxFOV * alpha, dt);
		}
	}

	void OnDestroy() override { }

	ColorRGBA GetColorAt(const float& u, const float& v) const override {
		return sprite.GetColorAt(u, v);
	}

	unsigned char GetAlphaAt(const float& u, const float& v) const override {
		return sprite.GetAlphaAt(u, v);
	}
};