#pragma once
#include "BasePlanet.h"
#include "../BasePhysicalObject.h"
#include "../Scene.h"

class PlanetMars : public BasePlanet {
private:
	std::string GetSpritePath() const override {
		return "./assets/planet02.png";
	}

public:
	PlanetMars(GamesEngineeringBase::Window& canvas)
		: BasePlanet(canvas) {
		scale = Vector2(75, 75);
		rotation = 0;
		mass = 5;
		renderOrder = RenderOrder::Second;
		canCollide = true;
		collisionRadius = 30;
		isGravitySource = true;
		isStatic = false;
		rotationSpeed = PI / 2.f;

		orbitHeight = 450;
	}
};