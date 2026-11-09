#pragma once
#include "src/BasePhysicalObject.h"
#include "GamesEngineeringBase.h"
#include "src/MyMath.h"
#include "src/Scene.h"

class TestPlanet : public BasePhysicalObject {
public:

	TestPlanet(GamesEngineeringBase::Window& canvas) : BasePhysicalObject(canvas) {
		scale = Vector2(200, 200); // a square
		globalPosition = { 200, 200 };
		maxVelocity = 400;
		rotation = 0;
		mass = 100;
		renderOrder = RenderOrder::Third;
		prevState = { globalPosition, scale, rotation };
		curState = { globalPosition, scale, rotation };
		canCollide = true;
		collisionRadius = 100;
		isGravitySource = true;
	}

	void OnStart() override {}

	void FixedUpdate(const float& dt) override {
		force = scene->GetGravityOn(this);
		BasePhysicalObject::UpdatePhysicPosition(dt);
	}

	void VisualUpdate(const float& dt) override {}

	void OnDestroy() override {
		
	}

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