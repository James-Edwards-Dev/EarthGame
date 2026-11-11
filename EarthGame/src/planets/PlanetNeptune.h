#pragma once
#include "BasePlanet.h"

class PlanetNeptune : public BasePlanet {
private:
	std::string GetSpritePath() const override {
		return "./assets/planet07.png";
	}

public:
	PlanetNeptune(GamesEngineeringBase::Window& canvas)
		: BasePlanet(canvas) {
		scale = Vector2(150, 150);
		rotation = 0;
		mass = 500;
		renderOrder = RenderOrder::Second;
		canCollide = true;
		collisionRadius = 60;
		isGravitySource = true;
		isStatic = false;
		rotationSpeed = PI / 6.f;

		orbitHeight = 2000;
	}
};