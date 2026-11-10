#pragma once
#include "../BasePhysicalObject.h"
#include "../../GamesEngineeringBase.h"
#include <memory>
#include <random>
#include "../Sprite.h"
#include "../Scene.h"

class BasePlanet : public BasePhysicalObject {
protected:
	std::weak_ptr<BasePlanet> parentPlanet;
	float orbitHeight;
	Sprite sprite;
	virtual std::string GetSpritePath() const = 0;

public:
	BasePlanet(GamesEngineeringBase::Window& canvas) : BasePhysicalObject(canvas) {
		scale = Vector2(25, 25);
		globalPosition = { 0, 0 };
		maxVelocity = 200;
		rotation = 0;
		mass = 100;
		renderOrder = RenderOrder::Second;
		prevState = { globalPosition, scale, rotation };
		curState = { globalPosition, scale, rotation };
		canCollide = true;
		collisionRadius = 10;
		isGravitySource = true;
		isStatic = false;

		orbitHeight = 0;
	}

	void OnStart() override {
		sprite.Load(GetSpritePath());
		if (auto p = parentPlanet.lock()) {
			float speed = std::sqrt(GRAVITY_CONSTANT * p->mass / orbitHeight);
			std::random_device rd;
			std::mt19937 rng(rd());
			std::uniform_real_distribution<> dis(0.0, TWO_PI);
			double phase = dis(rng);

			TeleportTo(p->globalPosition + Vector2{(float)std::cos(phase) * orbitHeight, (float)std::sin(phase) * orbitHeight});
			curPhysicState.velocityS = 
				p->curPhysicState.velocityS + Vector2{ (float)std::sin(phase) * speed, -(float)std::cos(phase) * speed };
			velocity = curPhysicState.velocityS;
		}
	}

	void FixedUpdate(const float& dt) override {
		force = scene->GetGravityOn(this);
		BasePhysicalObject::UpdatePhysicPosition(dt);
	}

	void VisualUpdate(const float& dt) override {}

	void OnDestroy() override {}

	// this needs to be set before adding to the scene
	void SetParentPlanet(std::weak_ptr<BasePlanet> p) { parentPlanet = p; }

	ColorRGBA GetColorAt(const float& u, const float& v) const override {
		return sprite.GetColorAt(u, v);
	}

	unsigned char GetAlphaAt(const float& u, const float& v) const override {
		return sprite.GetAlphaAt(u, v);
	}
};