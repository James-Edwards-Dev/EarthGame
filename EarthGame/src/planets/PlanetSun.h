#pragma once
#include "BasePlanet.h"

class PlanetSun : public BasePlanet {
private:
	std::string GetSpritePath() const override {
		return "./assets/planet00.png";
	}

public:
	PlanetSun(GamesEngineeringBase::Window& canvas) : BasePlanet(canvas) {
		scale = Vector2(50, 50);
		globalPosition = { 0, 0 };
		maxVelocity = 100;
		rotation = 0;
		mass = 900;
		renderOrder = RenderOrder::Second;
		prevState = { globalPosition, scale, rotation };
		curState = { globalPosition, scale, rotation };
		canCollide = true;
		collisionRadius = 20;
		isGravitySource = true;
		isStatic = false;

		orbitHeight = 0;
	}
};