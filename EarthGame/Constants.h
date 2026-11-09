#pragma once
#include <string>

const float FIXED_DELTATIME = 1.0f / 50.0f;
const short WINDOW_WIDTH = 1280;
const short WINDOW_HEIGHT = 720;
inline float FOV = 720.f; // the verticle length of window in game global axis
const float ASPECT_RATIO = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;
const float PI = 3.14159265358979323846f;
const float TWO_PI = 2.0f * PI;
const float INV255 = 1.0f / 255.0f;
const float INV_WINDOW_WIDTH = 1.f / WINDOW_WIDTH;
const float INV_WINDOW_HEIGHT = 1.f / WINDOW_HEIGHT;

inline float StepX() {return FOV * ASPECT_RATIO * INV_WINDOW_WIDTH;}
inline float StepY() {return FOV * INV_WINDOW_HEIGHT;}

const short RENDER_TILE_WIDTH = 64;
const short RENDER_TILE_HEIGHT = 64;

const int CO_RENDER_THREADS = 3;
