#pragma once

#include "../GamesEngineeringBase.h"
#include "SmallStructs.h"
#include <string>
#include <algorithm>

class Sprite {
private:
	GamesEngineeringBase::Image image;
	bool loaded;

public:
	bool Load(const std::string& path) {
		loaded = image.load(path);
		return loaded;
	}

	bool IsLoaded() const {
		return loaded;
	}

    ColorRGBA GetColorAt(const float& u, const float& v) const {
        ColorRGBA color;

        if (!loaded || image.width == 0 || image.height == 0) {
            return color;
        }

        float uu = std::clamp(u, 0.f, 1.f);
        float vv = std::clamp(v, 0.f, 1.f);

        int x = (int)(u * image.width);
        int y = (int)(v * image.height);

        x = std::min(x, (int)image.width - 1);
        y = std::min(y, (int)image.height - 1);

        auto ptr = image.atUnchecked(x, y);

        color.r = ptr[0];
        color.g = ptr[1];
        color.b = ptr[2];
        color.a = image.alphaAtUnchecked(x, y);

        return color;
    }

    unsigned char GetAlphaAt(const float& u, const float& v) const {
        if (!loaded || image.width == 0 || image.height == 0) {
            return 0;
        }

        float uu = std::clamp(u, 0.f, 1.f);
        float vv = std::clamp(v, 0.f, 1.f);

        int x = (int)(u * image.width);
        int y = (int)(v * image.height);

        x = std::min(x, (int)image.width - 1);
        y = std::min(y, (int)image.height - 1);

        return image.alphaAtUnchecked(x, y);
    }
};