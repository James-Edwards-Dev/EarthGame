#pragma once
#include "BasePlanet.h"

class PlanetJupiter : public BasePlanet {
private:
	std::string GetSpritePath() const override {
		return "./assets/planet05.png";
	}

public:
	PlanetJupiter(GamesEngineeringBase::Window& canvas)
		: BasePlanet(canvas) {
		scale = Vector2(150, 150);
		rotation = 0;
		mass = 100;
		renderOrder = RenderOrder::Second;
		canCollide = true;
		collisionRadius = 60;
		isGravitySource = true;
		isStatic = false;
		isClockwise = false;

		orbitHeight = 500;
	}
};