#pragma once
#include "BasePlanet.h"
#include "../LaunchController.h"
#include "../../GamesEngineeringBase.h"
#include "../MyMath.h"
#include "../PolyLine2D.h"
#include "../Scene.h"

class PlanetEarth : public BasePlanet {
private:
	LaunchController launchController;
	float minFOV = 120;
	float maxFOV = 1920;
	PolyLine2D* dirLine;

	std::string GetSpritePath() const override {
		return "./assets/planet03.png";
	}

public:
	PlanetEarth(GamesEngineeringBase::Window& canvas) 
		: BasePlanet(canvas), launchController(canvas, this) {
		scale = Vector2(75, 75);
		maxVelocity = 1000;
		rotation = 0;
		mass = 3;
		renderOrder = RenderOrder::Third;
		canCollide = true;
		collisionRadius = 30;
		isGravitySource = true;
		isStatic = false;
		dirLine = nullptr;

		orbitHeight = 150;
	}

	void OnStart() override {
		BasePlanet::OnStart();
		auto ptr = std::make_shared<PolyLine2D>(env);
		ptr->thickness = 3;
		ptr->lineColor = { 200,200,200,255 };;
		dirLine = ptr.get();
		scene->AddObject(ptr);
	}

	void FixedUpdate(const float& dt) override {
		force = scene->GetGravityOn(this);
		BasePhysicalObject::UpdatePhysicPosition(dt);
	}

	void VisualUpdate(const float& dt) override {
		launchController.UpdateInput();
		LaunchState s = launchController.GetState();
		if (s == LaunchState::Flying) {
			float alpha = std::clamp(
				(globalPosition-parentPlanet.lock()->globalPosition).Length(),
				0.f,
				900.f)
				/ 900.f;
			FOV = MyMath::Lerp(FOV, minFOV * (1 - alpha) + maxFOV * alpha, dt * 1.5);
		}
		else if (s == LaunchState::Aiming) {
			dirLine->points.clear();
			Vector2 dir = launchController.GetLaunchDrag().Normalized();
			Vector2 speedDelta = dir * launchController.launchStrength / mass;
			speedDelta = speedDelta + curPhysicState.velocityS;
			if (speedDelta.Length() > 1e-4f) {
				dirLine->points.push_back(globalPosition);
				dirLine->points.push_back(globalPosition + speedDelta.Normalized()
					* std::clamp(speedDelta.Length(), 50.f, 200.f));
			}
		}
	}

	void OnRelease() override {
		dampValue = 0.9f;
		dirLine->isPendingDestroy = true;
	}
};