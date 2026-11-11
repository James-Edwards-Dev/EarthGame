#pragma once
#include "BasePlanet.h"

class PlanetSun : public BasePlanet {
private:
	std::string GetSpritePath() const override {
		return "./assets/sun-planet.png";
	}

public:
	PlanetSun(GamesEngineeringBase::Window& canvas) : BasePlanet(canvas) {
		scale = Vector2(500, 500);
		globalPosition = { 0, 0 };
		maxVelocity = 100;
		rotation = 0;
		mass = 9000;
		renderOrder = RenderOrder::Third;
		prevState = { globalPosition, scale, rotation };
		curState = { globalPosition, scale, rotation };
		canCollide = true;
		collisionRadius = 120;
		isGravitySource = true;
		isStatic = true;
		isTrigger = true;

		orbitHeight = 0;
	}
};