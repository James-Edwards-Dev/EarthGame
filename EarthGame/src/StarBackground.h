#pragma once

#include "BaseGameObject.h"
#include "Sprite.h"
#include "../Constants.h"
#include <iostream>

class StarBackground : public BaseGameObject {
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
		float uu;
		float vv;
		uu = (u - floor(u / scale.x) * scale.x) / scale.x;
		vv = (v - floor(v / scale.y) * scale.y) / scale.y;

		return sprite.GetColorAt(uu, vv);
	}

	unsigned char GetAlphaAt(const float& u, const float& v) const override {
		float uu;
		float vv;
		uu = (u - floor(u / scale.x) * scale.x) / scale.x;
		vv = (v - floor(v / scale.y) * scale.y) / scale.y;
		return sprite.GetAlphaAt(uu, vv);
	}

	virtual bool DepthTest(float alpha, const Camera& camera, const short& curDepth, const ScreenAABB& tile) const override {
		return true;
	}

	virtual void RenderSelf(float alpha, const Camera& camera, const short& curDepth, const ScreenAABB& tile) const override {
		unsigned char* backBuffer = env.getBackBuffer();
		ColorRGBA color;

		for (int j = tile.minY; j <= tile.maxY; j++) {
			for (int i = tile.minX; i <= tile.maxX; i++) {
				Vector2 camera_pos = camera.GetGlobalPosition(i, j) * 0.6f;

				Vector2 global_pos = { (float)i, (float)j };
				Vector2 layer_1_pos = camera_pos * 0.3f + global_pos * 0.7f;
				Vector2 layer_2_pos = camera_pos * 0.1f + global_pos * 0.9f;

				ColorRGBA color_1 = GetColorAt(layer_1_pos.x, layer_1_pos.y);
				ColorRGBA color_2 = GetColorAt(layer_2_pos.x, layer_2_pos.y);

				color = {
					std::max(color_1.r, color_2.r),
					std::max(color_1.g, color_2.g),
					std::max(color_1.b, color_2.b),
					std::max(color_1.a, color_2.a) };

				camera.DrawOnCanvas(env, backBuffer, i, j, color);
			}
		}
	}
};