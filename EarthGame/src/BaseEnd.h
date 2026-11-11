#pragma once
#include "BaseGameObject.h"
#include "../GamesEngineeringBase.h"
#include "Sprite.h"
#include <string>
#include "Scene.h"
#include "SmallStructs.h"
#include "../Constants.h"

class BaseEnd : public BaseGameObject {
protected:
	Sprite sprite;
	SceneType type;
	bool prevMouseLeftPressed;
	std::string GetSpritePath() {
		switch (type) {
		case SceneType::Start:
			return "./assets/ends/Start.png";
		case SceneType::HesitateEnd:
			return "./assets/ends/HesitateEnd.png";
		case SceneType::IntoTheSunEnd:
			return "./assets/ends/IntoTheSunEnd.png";
		case SceneType::JupiterEnd:
			return "./assets/ends/JupiterEnd.png";
		case SceneType::MarsEnd:
			return "./assets/ends/MarsEnd.png";
		case SceneType::NeptuneEnd:
			return "./assets/ends/NeptuneEnd.png";
		case SceneType::TheEyeEnd:
			return "./assets/ends/TheEyeEnd.png";
		case SceneType::CometEnd:
			return "./assets/ends/CometEnd.png";
		case SceneType::HiddenEnd:
			return "./assets/ends/WormholeEnd.png";
		default:
			return "./assets/ends/HesitateEnd.png";
		}
	}

public:
	BaseEnd(GamesEngineeringBase::Window& win, const SceneType& t) : BaseGameObject(win) {
		globalPosition = { 0,0 };
		scale = { 1280,720 };
		rotation = 0;
		curState = { globalPosition, scale, rotation };
		prevState = curState;
		type = t;
		prevMouseLeftPressed = env.mouseButtonPressed(GamesEngineeringBase::MouseLeft);
	}

	void OnStart() override {
		sprite.Load(GetSpritePath());
	}

	void FixedUpdate(const float& dt) override {
		bool curMouseLeftPressed = env.mouseButtonPressed(GamesEngineeringBase::MouseLeft);
		if (curMouseLeftPressed && !prevMouseLeftPressed) {
			scene->ChangeScene(SceneType::Gameplay);
		}
		prevMouseLeftPressed = curMouseLeftPressed;
	}
	void VisualUpdate(const float& dt) override {}
	void OnDestroy() override {}

	ColorRGBA GetColorAt(const float& u, const float& v) const override {
		return sprite.GetColorAt(u, v);
	}

	unsigned char GetAlphaAt(const float& u, const float& v) const override {
		return sprite.GetAlphaAt(u, v);
	}
};