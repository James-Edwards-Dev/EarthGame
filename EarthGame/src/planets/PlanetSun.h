#pragma once
#include "BasePlanet.h"

class PlanetSun : public BasePlanet {
private:
	std::string GetSpritePath() const override {
		return "./assets/planet00.png";
	}

public:
	PlanetSun(GamesEngineeringBase::Window& canvas) : BasePlanet(canvas) {
		scale = Vector2(200, 200);
		globalPosition = { 0, 0 };
		maxVelocity = 100;
		rotation = 0;
		mass = 4000;
		renderOrder = RenderOrder::Second;
		prevState = { globalPosition, scale, rotation };
		curState = { globalPosition, scale, rotation };
		canCollide = true;
		collisionRadius = 80;
		isGravitySource = true;
		isStatic = true;

		orbitHeight = 0;
	}
};