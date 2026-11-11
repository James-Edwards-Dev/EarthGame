#pragma once

#include "BaseGameObject.h"
#include "Sprite.h"
#include "../Constants.h"
#include <iostream>

class StarBackground : public BaseGameObject 
{
	Sprite sprite;
public:
	StarBackground(GamesEngineeringBase::Window& win) : BaseGameObject(win) {
		globalPosition = { 0,0 };
		scale = { 640, 360 };
		rotation = { 0 };
		curState = { globalPosition, scale, rotation };
		renderOrder = RenderOrder::First;
	}

	virtual void OnStart() override {
		sprite.Load("./assets/star-background.png");
	}
	virtual void FixedUpdate(const float& dt) {}
	void VisualUpdate(const float& dt) override {}
	void OnDestroy() override {}

	ColorRGBA GetColorAt(const float& u, const float& v) const override {
		int u2 = static_cast<int>(u) % static_cast<int>(scale.x);
		int v2 = static_cast<int>(v) % static_cast<int>(scale.x);

		return sprite.GetColorAt(u2, v2);
	}

	unsigned char GetAlphaAt(const float& u, const float& v) const override {
		float u2 = u - floor(u / scale.x) * scale.x;
		float v2 = v - floor(v / scale.y) * scale.y;
		return sprite.GetAlphaAt(u2, v2);
	}

	virtual bool DepthTest(float alpha, const Camera& camera, const short& curDepth, const ScreenAABB& tile) const override {
		return true;
	}

	virtual void RenderSelf(float alpha, const Camera& camera, const short& curDepth, const ScreenAABB& tile) const override {
		unsigned char* backBuffer = env.getBackBuffer();
		ColorRGBA color;

		for (int j = tile.minY; j <= tile.maxY; j++) {
			for (int i = tile.minX; i <= tile.maxX; i++) {

				color = GetColorAt(i, j);
				camera.DrawOnCanvas(env, backBuffer, i, j, color);
			}
		}
	}
};