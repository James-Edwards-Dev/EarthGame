#include "GamesEngineeringBase.h"
#include "Constants.h"
#include "src/Camera.h"
#include "src/Scene.h"
#include "TestObject.h"
#include "TestPlanet.h"
#include <iostream>

int main() {
	bool running = true;
	GamesEngineeringBase::Window canvas;
	canvas.create(WINDOW_WIDTH, WINDOW_HEIGHT, "Earth Game");
	Camera camera;
	Scene mainScene = Scene(canvas, camera);
	auto ptr = std::make_shared<TestObject>(canvas);
	mainScene.AddObject(ptr);
	mainScene.SetCameraTarget(ptr.get());
	mainScene.AddObject(std::make_shared<TestPlanet>(canvas));

#pragma region Timers
	GamesEngineeringBase::Timer loopTimer;
	float frameTime;
	float frameTimeAccumulator = 0;
	int frameCounter = 0;
	float frameTimeCounter = 0;

	GamesEngineeringBase::Timer performanceTestTimer;
	float updateTime = 0;
	float clearTime = 0;
	float renderTime = 0;
	float presentTime = 0;
	//int fpsCount = 0;
#pragma endregion

	while (running) {
		frameTime = loopTimer.dt();

#pragma region PerformanceDetect
		frameTimeCounter += frameTime;
		if (frameTimeCounter > 1.0f) {
			std::cout << "FPS: " << frameCounter / frameTimeCounter << std::endl;
			std::cout << "update time: " << updateTime / frameTimeCounter << std::endl;
			std::cout << "clear time: " << clearTime / frameTimeCounter << std::endl;
			std::cout << "render time: " << renderTime / frameTimeCounter << std::endl;
			std::cout << "present time: " << presentTime / frameTimeCounter << std::endl;
			frameTimeCounter = 0;
			frameCounter = 0;
			updateTime = 0;
			clearTime = 0;
			renderTime = 0;
			presentTime = 0;
		}
		frameCounter += 1;
#pragma endregion

		if (frameTime > 0.1f) { // to avoid enormous dt
			frameTime = 0.1f;
		}
		frameTimeAccumulator += frameTime;

#pragma region FixedUpdate
		performanceTestTimer.reset();
		while (frameTimeAccumulator >= FIXED_DELTATIME) {
			// traverse the fixed_updated objects
			// store the previous state
			// update the current state using FIXED_DELTATIME
			mainScene.FixedUpdateObjects(FIXED_DELTATIME);
			mainScene.RecycleObjects();
			frameTimeAccumulator -= FIXED_DELTATIME;
		}
#pragma endregion

#pragma region VisualUpdate
		// traverse the objects, update them by frameTime
		mainScene.VisualUpdateObjects(frameTime);
		camera.VisualUpdate(frameTime);
		updateTime += performanceTestTimer.dt();
#pragma endregion

#pragma region Render
		clearTime += performanceTestTimer.dt();
		// traverse the render-able objects, sort them by order, 
		// then call render method respectively.
		canvas.clear();
		mainScene.RenderObjects(frameTimeAccumulator / FIXED_DELTATIME);
		canvas.present();
		renderTime += performanceTestTimer.dt();
		presentTime += performanceTestTimer.dt();
#pragma endregion

		if (canvas.keyPressed(0x1b)) { // esc to close
			running = false;
		}
	}
	return 0;
}