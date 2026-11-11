#pragma once
#include "BasePlanet.h"
#include "../BasePhysicalObject.h"
#include "../Scene.h"

class PlanetComet : public BasePlanet {
private:
	std::string GetSpritePath() const override {
		return "./assets/planet04.png";
	}

public:
	PlanetComet(GamesEngineeringBase::Window& canvas)
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
		isComet = true;

		orbitHeight = 1600;
	}
};