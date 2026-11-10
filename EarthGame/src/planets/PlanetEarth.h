#pragma once
#include "BasePlanet.h"
#include "../LaunchController.h"
#include "../../GamesEngineeringBase.h"
#include "../MyMath.h"

class PlanetEarth : public BasePlanet {
private:
	LaunchController launchController;
	float minFOV = 360;
	float maxFOV = 1920;

	std::string GetSpritePath() const override {
		return "./assets/planet03.png";
	}

public:
	PlanetEarth(GamesEngineeringBase::Window& canvas) 
		: BasePlanet(canvas), launchController(canvas, this) {
		scale = Vector2(25, 25);
		maxVelocity = 1000;
		rotation = 0;
		mass = 3;
		renderOrder = RenderOrder::Third;
		canCollide = true;
		collisionRadius = 10;
		isGravitySource = true;
		isStatic = false;

		orbitHeight = 150;
	}

	void FixedUpdate(const float& dt) override {
		force = scene->GetGravityOn(this);
		BasePhysicalObject::UpdatePhysicPosition(dt);
	}

	void VisualUpdate(const float& dt) override {
		launchController.UpdateInput();
		if (launchController.GetState() == LaunchState::Flying) {
			float alpha = std::clamp(velocity.Length(), 0.f, maxVelocity) / maxVelocity;
			FOV = MyMath::Lerp(FOV, minFOV * (1 - alpha) + maxFOV * alpha, dt * 1.5);
		}
	}
};