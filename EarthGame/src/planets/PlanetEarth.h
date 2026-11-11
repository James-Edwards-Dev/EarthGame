#pragma once
#include "BasePlanet.h"
#include "../LaunchController.h"
#include "../../GamesEngineeringBase.h"
#include "../MyMath.h"
#include "../PolyLine2D.h"
#include "../Scene.h"
#include "PlanetJupiter.h"
#include "PlanetSun.h"
#include "PlanetMars.h"
#include "PlanetNeptune.h"
#include "../ObjectTimer.h"
#include "PlanetComet.h"

class PlanetEarth : public BasePlanet {
private:
	LaunchController launchController;
	float minFOV = 120;
	float maxFOV = 3000;
	PolyLine2D* dirLine;
	float waitTime = 0;
	float hesitateWaitTime = 30;
	float solarSystemRadius = 3500;
	bool hit;

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
		renderOrder = RenderOrder::Second;
		canCollide = true;
		collisionRadius = 40;
		isGravitySource = true;
		isStatic = false;
		dirLine = nullptr;
		selfTurning = true;
		rotationSpeed = PI;

		orbitHeight = 250;
		hit = false;
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
		BasePlanet::FixedUpdate(dt);
	}

	void VisualUpdate(const float& dt) override {
		BasePlanet::VisualUpdate(dt);
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
		if (hit) {
			return;
		}
		
		if (auto ptr = dynamic_cast<PlanetSun*>(other)) {
			auto ptr1 = std::make_shared<ObjectTimer>(env);
			ptr1->SetTimer(1, [this]() {
				scene->ChangeScene(SceneType::IntoTheSunEnd); });
			ptr1->StartTimer();
			scene->AddObject(ptr1);
			hit = true;
			return;
		}
		if (auto ptr = dynamic_cast<PlanetJupiter*>(other)) {
			auto ptr1 = std::make_shared<ObjectTimer>(env);
			ptr1->SetTimer(1, [this]() {
				scene->ChangeScene(SceneType::JupiterEnd); });
			ptr1->StartTimer();
			scene->AddObject(ptr1);
			hit = true;
			return;
		}
		if (auto ptr = dynamic_cast<PlanetMars*>(other)) {
			auto ptr1 = std::make_shared<ObjectTimer>(env);
			ptr1->SetTimer(1, [this]() {
				scene->ChangeScene(SceneType::MarsEnd); });
			ptr1->StartTimer();
			scene->AddObject(ptr1);
			hit = true;
			return;
		}
		if (auto ptr = dynamic_cast<PlanetNeptune*>(other)) {
			auto ptr1 = std::make_shared<ObjectTimer>(env);
			ptr1->SetTimer(1, [this]() {
				scene->ChangeScene(SceneType::NeptuneEnd); });
			ptr1->StartTimer();
			scene->AddObject(ptr1);
			hit = true;
			return;
		}
		if (auto ptr = dynamic_cast<PlanetComet*>(other)) {
			auto ptr1 = std::make_shared<ObjectTimer>(env);
			ptr1->SetTimer(1, [this]() {
				scene->ChangeScene(SceneType::CometEnd); });
			ptr1->StartTimer();
			scene->AddObject(ptr1);
			hit = true;
			return;
		}
	}
};