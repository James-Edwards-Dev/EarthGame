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
		scale = Vector2(125, 125);
		rotation = 0;
		mass = 200;
		renderOrder = RenderOrder::Second;
		canCollide = true;
		collisionRadius = 50;
		isGravitySource = true;
		isStatic = false;

		orbitHeight = 1500;
	}
};