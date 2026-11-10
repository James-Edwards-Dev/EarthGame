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
		scale = Vector2(200, 200);
		rotation = 0;
		mass = 200;
		renderOrder = RenderOrder::Second;
		canCollide = true;
		collisionRadius = 80;
		isGravitySource = true;
		isStatic = false;

		orbitHeight = 900;
	}
};