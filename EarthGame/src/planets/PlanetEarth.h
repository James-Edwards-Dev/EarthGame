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
	float maxFOV = 3000;
	PolyLine2D* dirLine;
	float waitTime = 0;
	float hesitateWaitTime = 30;
	float solarSystemRadius = 3500;

	std::string GetSpritePath() const override {
		return "./assets/planet03.png";
	}

public:
	PlanetEarth(GamesEngineeringBase::Window& canvas) 
		: BasePlanet(canvas), launchController(canvas, this) {
		scale = Vector2(100, 100);
		maxVelocity = 1e10;
		rotation = 0;
		mass = 20;
		renderOrder = RenderOrder::Third;
		canCollide = true;
		collisionRadius = 40;
		isGravitySource = true;
		isStatic = false;
		dirLine = nullptr;

		orbitHeight = 300;
	}

	void OnStart() override {
		BasePlanet::OnStart();
		auto ptr = std::make_shared<PolyLine2D>(env);
		ptr->thickness = 20.f;
		ptr->lineColor = { 200,200,200,255 };
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
				1400.f)
				/ 1400.f;
			FOV = MyMath::Lerp(FOV, minFOV * (1 - alpha) + maxFOV * alpha, dt * 1.5);

			float distanceToSun = (parentPlanet.lock()->globalPosition - globalPosition).Length();
			if (distanceToSun > solarSystemRadius) {
				scene->ChangeScene(SceneType::TheEyeEnd);
			}
		}
		else if (s == LaunchState::Aiming) {
			waitTime += dt;
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
		else {
			waitTime += dt;
			FOV = MyMath::Lerp(FOV, 1080.f, dt * 0.5f);
		}

		if (waitTime >= hesitateWaitTime) {
			scene->ChangeScene(SceneType::HesitateEnd);
		}
	}

	void OnRelease() override {
		//dampValue = 0.95f;
		dirLine->isPendingDestroy = true;
	}

	void OnCollision(BaseGameObject* other, const Vector2& normal, const Vector2& position) override {

	}
};