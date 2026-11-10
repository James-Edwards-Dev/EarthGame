#pragma once
#include "BasePlanet.h"

class PlanetJupiter : public BasePlanet {
private:
	std::string GetSpritePath() const override {
		return "./assets/planet08.png";
	}

public:
	PlanetJupiter(GamesEngineeringBase::Window& canvas)
		: BasePlanet(canvas) {
		scale = Vector2(50, 50);
		rotation = 0;
		mass = 20;
		renderOrder = RenderOrder::Second;
		canCollide = true;
		collisionRadius = 20;
		isGravitySource = true;
		isStatic = false;

		orbitHeight = 450;
	}
};