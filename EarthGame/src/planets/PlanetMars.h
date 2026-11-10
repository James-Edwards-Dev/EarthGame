#pragma once
#include "BasePlanet.h"

class PlanetMars : public BasePlanet {
private:
	std::string GetSpritePath() const override {
		return "./assets/planet02.png";
	}

public:
	PlanetMars(GamesEngineeringBase::Window& canvas)
		: BasePlanet(canvas) {
		scale = Vector2(50, 50);
		rotation = 0;
		mass = 2;
		renderOrder = RenderOrder::Second;
		canCollide = true;
		collisionRadius = 20;
		isGravitySource = true;
		isStatic = false;

		orbitHeight = 250;
	}
};