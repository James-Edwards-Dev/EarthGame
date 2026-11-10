#pragma once
#include "src/BasePhysicalObject.h"
#include "GamesEngineeringBase.h"
#include "src/Sprite.h"

class TestObject : public BasePhysicalObject {
private:
	bool launcherLocked = true;
	Vector2 launcherAnchor;
public:
	float maxForce;
	Sprite sprite;

	TestObject(GamesEngineeringBase::Window& canvas) : BasePhysicalObject(canvas) {
		scale = Vector2(100, 100); // a square
		rotation = 0;
		mass = 300;
		maxForce = 400 * mass; // global position per second
		maxVelocity = 400;
		renderOrder = RenderOrder::Third;
		prevState = { globalPosition, scale, rotation };
		curState = { globalPosition, scale, rotation };
		canCollide = true;
		collisionRadius = 40;
		isGravitySource = true;
		launcherAnchor = globalPosition;
	}

	

	void OnStart() override {
		sprite.Load("./assets/planet00.png");
	}

	void FixedUpdate(const float& dt) override {
		Vector2 inputDir = Vector2();
		if (env.keyPressed(0x57)) { // W
			inputDir.y += -1;
		}
		if (env.keyPressed(0x41)) { // A
			inputDir.x += -1;
		}
		if (env.keyPressed(0x53)) { // S
			inputDir.y += 1;
		}
		if (env.keyPressed(0x44)) { // D
			inputDir.x += 1;
		}

		if (inputDir.Length() > 0) {
			inputDir = inputDir.Normalized();
		}

		force = inputDir * maxForce;
		force = force + scene->GetGravityOn(this);
		BasePhysicalObject::UpdatePhysicPosition(dt);
	}


	//lock position
	void SolveConstraints() override {
		if (launcherLocked)//launcher locked
			globalPosition = launcherAnchor;
	}

	void ApplyLaunchImpulse(const Vector2& impulse) {
		Vector2 deltaVelocity;
		if (mass <= 0) return;
		deltaVelocity = impulse / mass;		//deltav = J/m
		curPhysicState.velocityS = curPhysicState.velocityS+deltaVelocity;
		velocity = curPhysicState.velocityS;
	}

	void SetLauncherLocked(bool locked) {
		launcherLocked = locked;
	}

	void ResetForLaunch(const Vector2& position) {
		launcherLocked = true;
		launcherAnchor = position;
		globalPosition = position;
		velocity = Vector2();
		force = Vector2();
		ResetStates();
	}

	void VisualUpdate(const float& dt) override { }

	void OnDestroy() override { }

	ColorRGBA GetColorAt(const float& u, const float& v) const override {
		return sprite.GetColorAt(u, v);
	}

	unsigned char GetAlphaAt(const float& u, const float& v) const override {
		return sprite.GetAlphaAt(u, v);
	}
};