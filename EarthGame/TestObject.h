#pragma once
#include "src/BasePhysicalObject.h"
#include "GamesEngineeringBase.h"
#include "src/MyMath.h"

class TestObject : public BasePhysicalObject {
public:
	float maxForce;
	float rotationSpeed;
	float zoomSpeed;

	TestObject(GamesEngineeringBase::Window& canvas) : BasePhysicalObject(canvas) {
		scale = Vector2(100, 100); // a square
		rotation = 0;
		mass = 30;
		maxForce = 400 * mass; // global position per second
		maxVelocity = 400;
		rotationSpeed = PI; // radians per second
		zoomSpeed = 100; // global position per second
		renderOrder = RenderOrder::Third;
		prevState = { globalPosition, scale, rotation };
		curState = { globalPosition, scale, rotation };
		canCollide = true;
		collisionRadius = 50;
		isGravitySource = true;
	}

	void OnStart() override { }

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

		float inputAngle = 0;
		if (env.keyPressed(0x51)) { // Q
			inputAngle += 1;
		}
		if (env.keyPressed(0x45)) { // E
			inputAngle -= 1;
		}

		float inputZoomH = 0;
		float inputZoomV = 0;
		if (env.keyPressed(0x5a)) { // z
			inputZoomH += 1;
		}
		if (env.keyPressed(0x58)) { // x
			inputZoomH -= 1;
		}
		if (env.keyPressed(0x43)) { // c
			inputZoomV += 1;
		}
		if (env.keyPressed(0x56)) { // v
			inputZoomV -= 1;
		}

		force = inputDir * maxForce;
		force = force + scene->GetGravityOn(this);
		rotation = MyMath::WrapAngle(rotation + inputAngle * rotationSpeed * dt);
		//scale = scale + Vector2(inputZoomH * zoomSpeed * dt, inputZoomV * zoomSpeed * dt);
		FOV = FOV + inputZoomH * zoomSpeed * dt;
		FOV = std::clamp(FOV, 100.f, 1000.f);
		BasePhysicalObject::UpdatePhysicPosition(dt);
	}

	void VisualUpdate(const float& dt) override { }

	void OnDestroy() override { }

	ColorRGBA GetColorAt(const float& u, const float& v) const override {
		ColorRGBA color;
		float uu = std::clamp(u, 0.0f, 1.0f);
		float vv = std::clamp(v, 0.0f, 1.0f);

		color.r = 255;
		color.g = 255;
		color.b = 255;
		color.a = GetAlphaAt(uu, vv);

		return color;
	}

	unsigned char GetAlphaAt(const float& u, const float& v) const override {
		if ((u - 0.5f) * (u - 0.5f) + (v - 0.5f) * (v - 0.5f) < 0.25f) {
			return 255;
		}
		else {
			return 0;
		}
	}
};