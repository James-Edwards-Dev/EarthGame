#pragma once
#include "BasePlanet.h"

class PlanetSattelite : public BasePlanet {
private:
	std::string GetSpritePath() const override {
		return "./assets/planet06.png";
	}

public:
	PlanetSattelite(GamesEngineeringBase::Window& canvas)
		: BasePlanet(canvas) {
		scale = Vector2(25, 25);
		maxVelocity = 1e5;
		rotation = 0;
		mass = 1;
		renderOrder = RenderOrder::Second;
		canCollide = true;
		collisionRadius = 10;
		isGravitySource = true;
		isStatic = false;
		isClockwise = false;

		orbitHeight = 100;
	}
};