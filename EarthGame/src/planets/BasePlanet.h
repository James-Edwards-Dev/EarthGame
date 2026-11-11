#pragma once
#include "../BasePhysicalObject.h"
#include "../../GamesEngineeringBase.h"
#include <memory>
#include <random>
#include "../Sprite.h"
#include "../Scene.h"
#include "../PolyLine2D.h"

class BasePlanet : public BasePhysicalObject {
protected:
	std::weak_ptr<BasePlanet> parentPlanet;
	float orbitHeight;
	Sprite sprite;
	virtual std::string GetSpritePath() const = 0;
	bool isClockwise;
	PolyLine2D* trail;
	float trailTick;
	float trailPointTime = 0.1f;
	int trailAmount = 40;

public:
	BasePlanet(GamesEngineeringBase::Window& canvas) : BasePhysicalObject(canvas) {
		scale = Vector2(25, 25);
		globalPosition = { 0, 0 };
		maxVelocity = 400;
		rotation = 0;
		mass = 10;
		renderOrder = RenderOrder::Second;
		prevState = { globalPosition, scale, rotation };
		curState = { globalPosition, scale, rotation };
		canCollide = true;
		collisionRadius = 10;
		isGravitySource = true;
		isStatic = false;
		isClockwise = true;
		trail = nullptr;
		trailTick = 0.f;

		orbitHeight = 0;
	}

	void OnStart() override {
		sprite.Load(GetSpritePath());
		if (auto p = parentPlanet.lock()) {
			float speed = std::sqrt(GRAVITY_CONSTANT / orbitHeight * p->mass);
			std::random_device rd;
			std::mt19937 rng(rd());
			std::uniform_real_distribution<> dis(0.0, TWO_PI);
			double phase = dis(rng);

			TeleportTo(p->globalPosition + Vector2{(float)std::cos(phase) * orbitHeight, (float)std::sin(phase) * orbitHeight});
			if (isClockwise) {
				curPhysicState.velocityS =
					p->curPhysicState.velocityS + Vector2{ (float)std::sin(phase) * speed, -(float)std::cos(phase) * speed };
			}
			else {
				curPhysicState.velocityS =
					p->curPhysicState.velocityS + Vector2{ -(float)std::sin(phase) * speed, (float)std::cos(phase) * speed };
			}
			
			velocity = curPhysicState.velocityS;
		}

		auto ptr = std::make_shared<PolyLine2D>(env);
		trail = ptr.get();
		ptr->lineColor = { 255,255,255,255 };
		ptr->renderOrder = RenderOrder::First;
		ptr->thickness = collisionRadius / 3;
		ptr->points.push_back(globalPosition);
		scene->AddObject(ptr);
	}

	void FixedUpdate(const float& dt) override {
		force = scene->GetGravityOn(this);
		BasePhysicalObject::UpdatePhysicPosition(dt);
	}

	void VisualUpdate(const float& dt) override {
		trailTick += dt;
		if (trailTick > trailPointTime) {
			trailTick -= trailPointTime;
			trail->points.push_back(globalPosition);
			if (trail->points.size() > trailAmount) {
				trail->points.erase(trail->points.begin());
			}
		}
	}

	void OnDestroy() override {
		trail->isPendingDestroy = true;
	}

	// this needs to be set before adding to the scene
	void SetParentPlanet(std::weak_ptr<BasePlanet> p) { parentPlanet = p; }

	ColorRGBA GetColorAt(const float& u, const float& v) const override {
		return sprite.GetColorAt(u, v);
	}

	unsigned char GetAlphaAt(const float& u, const float& v) const override {
		return sprite.GetAlphaAt(u, v);
	}
	
	virtual void OnRelease() {}
};