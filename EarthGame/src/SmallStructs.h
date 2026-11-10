#pragma once
#include "Vector2.h"
#include <string>

class BaseGameObject;
class BasePhysicalObject;

enum RenderOrder {
	First,
	Second,
	Third,
	HUD,
};

struct ColorRGBA {
	unsigned char r = 0;
	unsigned char g = 0;
	unsigned char b = 0;
	unsigned char a = 0;

	ColorRGBA() {
		r = 0;
		g = 0;
		b = 0;
		a = 0;
	}

	ColorRGBA(const int& r, const int& g, const int& b, const int& a) {
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}
};

struct RenderedItem {
	BaseGameObject* objPtr = nullptr;
	short depth = 0x7fff;
};

enum class SceneType {
	None,
	Start,
	Gameplay,
	HesitateEnd,
	IntoTheSunEnd,
	JupiterEnd,
	MarsEnd,
	NeptuneEnd,
	TheEyeEnd,
};