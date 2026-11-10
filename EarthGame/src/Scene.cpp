#include "Scene.h"
#include <thread>
#include <functional>
#include "BasePhysicalObject.h"
#include "Vector2.h"
//#include "../TestObject.h"
//#include "../TestPlanet.h"
#include "BaseEnd.h"
#include "planets/PlanetEarth.h"
#include "planets/PlanetSun.h"
#include "planets/PlanetJupiter.h"
#include "planets/PlanetMars.h"
#include "planets/PlanetNeptune.h"

Scene::Scene(GamesEngineeringBase::Window& window, Camera& camera) 
	: canvas(window), camera(camera) {
	// slice the canvas into tiles for multi-thread rendering
	ScreenAABB renderTile = { 0, 0, 0, 0 };
	for (short yPos = 0; yPos < WINDOW_HEIGHT; yPos += RENDER_TILE_HEIGHT) {
		for (short xPos = 0; xPos < WINDOW_WIDTH; xPos += RENDER_TILE_WIDTH) {
			renderTile.minX = xPos;
			renderTile.maxX = std::min(xPos + RENDER_TILE_WIDTH - 1, WINDOW_WIDTH - 1);
			renderTile.minY = yPos;
			renderTile.maxY = std::min(yPos + RENDER_TILE_HEIGHT - 1, WINDOW_HEIGHT - 1);
			renderTiles.push_back(renderTile);
		}
	}
	isPendingSceneChange = false;
	nextScene = SceneType::None;
}

void Scene::AddObject(std::shared_ptr<BaseGameObject> obj) {
	// no checking if already exists
	if (!obj) {
		return;
	}
	BaseGameObject* raw = obj.get();
	raw->scene = this;
	objects.push_back(obj);
	if (raw->isGravitySource) {
		auto physical = std::dynamic_pointer_cast<BasePhysicalObject>(obj);
		if (physical) {
			gravitySourceObjects.push_back(physical);
		}
	}
	raw->OnStart();

}

//void Scene::AddUIObject(std::shared_ptr<ClickableText> uiObject) {
//	if (!uiObject) {
//		return;
//	}
//	ClickableText* raw = uiObject.get();
//	uiObjects.PushBack(uiObject);
//	raw->OnStart();
//}

void Scene::FixedUpdateObjects(const float& dt) {
	// Position Based Dynamics, https://doi.org/10.1016/j.jvcir.2007.01.005
	// let a be acceleration, v be velocity, x be position, t be time, dt be delta time.
	// 1. get a(t+dt) from input
	// 2. damp velocity: v(t) * dampPercentPerSecond^dt -> v(t)
	// 3. v(t) + a(t)dt -> v(t+dt)
	// 4. x(t) + v(t+dt)dt -> p(t+dt)	(p for predicted x because it'll be adjusted afterwards)
	int size = objects.size();
	for (int i = 0; i < size; ++i) {
		if (!objects[i]->isActive || objects[i]->isPendingDestroy) {
			continue;
		}
		objects[i]->FixedUpdate(dt);
	}

	// 5. solve for physical constrains and collisions (here only 1 iteration):
	// p(t+dt) + constraintProjection -> p'(t+dt)
#pragma region ConstraintSolving
	// -1. solve entity colliding entity.
	for (int i = 0; i < size; ++i) {
		if (!objects[i]->isActive || objects[i]->isPendingDestroy) {
			continue;
		}
		for (int j = i + 1; j < size; ++j) {
			if (!objects[j]->isActive || objects[j]->isPendingDestroy) {
				continue;
			}
			auto o1 = objects[i].get();
			auto o2 = objects[j].get();
			if (!o1->canCollide || !o2->canCollide) {
				continue;
			}
			auto a = dynamic_cast<BasePhysicalObject*>(o1);
			auto b = dynamic_cast<BasePhysicalObject*>(o2);
			// maybe just let me know if there's an error
			//if (!a || !b) {
			//	continue;
			//}

			Vector2 delta = b->globalPosition - a->globalPosition;
			float dist = delta.Length();
			float minDist = b->collisionRadius + a->collisionRadius;

			if (dist >= minDist || dist < 1e-4f) { // if too close, that's an error, just wait until they break up
				continue;
			}

			float err = minDist - dist;
			Vector2 normal = delta.Normalized();
			Vector2 position = a->globalPosition * 0.5f + b->globalPosition * 0.5f;
			
			// apply collision constraint: move the position
			if (!a->isTrigger && !b->isTrigger) {
				float wa;
				float wb;
				if (a->isStatic) {
					wa = 0.f;
				}
				else {
					wa = 1.f / a->mass;
				}
				if (b->isStatic) {
					wb = 0.f;
				}
				else {
					wb = 1.f / b->mass;
				}
				float wSum = wa + wb;

				if (wSum > 1e-4f) {
					a->globalPosition = a->globalPosition - normal * err * (wa / wSum);
					b->globalPosition = b->globalPosition + normal * err * (wb / wSum);
				}
			}

			a->OnCollision(b, normal * -1, position);
			b->OnCollision(a, normal, position);
		}
	}
	
	// -2. solve other constraints here (distance, angle, ...)
	for (int i = 0; i < size; ++i) {
		if (!objects[i]->isActive || objects[i]->isPendingDestroy) {
			continue;
		}
		objects[i]->SolveConstraints();
	}
#pragma endregion

	// 6. update velocity: [p'(t+dt) - x(t)] / dt -> v(t+dt)
	// notice that this is an approximation, using v(t+dt/2) as v(t+dt)
	//	so it has an error of O(1)
	// a better solution called Velocity Verlet Integration is: 
	//	[a(t+dt) + a(t)] / 2 * dt + v(t) -> v(t+dt)
	// but it's not compatible with constraints projection, 
	//	since the projection uses an implicit force to move objects.
	for (int i = 0; i < size; ++i) {
		if (!objects[i]->isActive || objects[i]->isPendingDestroy) {
			continue;
		}
		objects[i]->UpdateVelocity(dt);
	}

	// 7. apply new pos: p'(t+dt) -> x(t+dt)
	for (int i = 0; i < size; ++i) {
		if (!objects[i]->isActive || objects[i]->isPendingDestroy) {
			continue;
		}
		objects[i]->UpdateStates();
	}

	if (canvas.keyPressed('R')) {
		ChangeScene(SceneType::Start);
	}
}

void Scene::VisualUpdateObjects(const float& dt) {
	int size = objects.size();
	for (int i = 0; i < size; ++i) {
		if (!objects[i]->isActive || objects[i]->isPendingDestroy) {
			continue;
		}
		objects[i]->VisualUpdate(dt);
	}

	//size = uiObjects.GetSize();
	//for (int i = 0; i < size; ++i) {
	//	if (!uiObjects[i] || uiObjects[i]->isPendingDestroy) {
	//		continue;
	//	}
	//	uiObjects[i]->VisualUpdate(dt);
	//}

	if (isPendingSceneChange) {
		RecycleAllObjects();
		InitiateChangedScene();
		isPendingSceneChange = false;
		nextScene = SceneType::None;
	}
}

void Scene::RenderObjects(const float& alpha) {
	int size = objects.size();
	camera.ClearDepthGraph();

	// 1. interpolate the renderState by prevState and curState, 
	//		because the visual update is not consistent with fixed update.
	for (int i = 0; i < size; ++i) {
		if (!objects[i]->isActive || objects[i]->isPendingDestroy) {
			continue;
		}
		objects[i]->UpdateRenderState(alpha, camera);
	}

	// 2. build threads to render
	std::atomic<int> nextTile = 0;
	std::thread workers[CO_RENDER_THREADS];

	for (int i = 0; i < CO_RENDER_THREADS; ++i) {
		workers[i] = std::thread(&Scene::RenderWorker, this, std::ref(alpha), std::ref(nextTile), std::ref(renderedItemLists[i]));
	}
	RenderWorker(alpha, nextTile, renderedItemLists[CO_RENDER_THREADS]);
	for (int i = 0; i < CO_RENDER_THREADS; ++i) {
		workers[i].join();
	}

	//// 3. render UI objects at last
	//size = uiObjects.GetSize();
	//for (int i = 0; i < size; ++i) {
	//	if (!uiObjects[i] || uiObjects[i]->isPendingDestroy) {
	//		continue;
	//	}
	//	uiObjects[i]->RenderSelf();
	//}
}

void Scene::RecycleObjects() {
	int size = objects.size();
	for (int i = size - 1; i >= 0; --i) {
		if (!objects[i]->isPendingDestroy) {
			continue;
		}
		// clear other places
		auto ptr = objects[i].get();
		if (camera.followTarget == ptr) {
			camera.followTarget = nullptr;
			camera.globalPosition = { 0,0 };
		}

		objects[i]->OnDestroy();
		objects.erase(objects.begin() + i);
	}

	// recycle UI objects
	//size = uiObjects.GetSize();
	//for (int i = size - 1; i >= 0; --i) {
	//	if (!uiObjects[i] || !uiObjects[i]->isPendingDestroy) {
	//		continue;
	//	}
	//	uiObjects[i]->OnDestroy();
	//	uiObjects.Erase(i);
	//}
}

void Scene::RecycleAllObjects() {
	// recycle the objects to trigger their OnDestroy function

	int size = objects.size();
	for (int i = size - 1; i >= 0; --i) {
		objects[i]->OnDestroy();
		objects.erase(objects.begin() + i);
	}

	//size = uiObjects.GetSize();
	//for (int i = size - 1; i >= 0; --i) {
	//	uiObjects[i]->OnDestroy();
	//	uiObjects.Erase(i);
	//}
}

void Scene::ChangeScene(const SceneType& nextScene) {
	isPendingSceneChange = true;
	this->nextScene = nextScene;
}

void Scene::InitiateChangedScene() {
	switch (nextScene) {
	case SceneType::None:
		break;
	case SceneType::Start: {
		auto ptr = std::make_shared<BaseEnd>(canvas, nextScene);
		SetCameraTarget(nullptr);
		camera.globalPosition = { 0,0 };
		FOV = 720;
		AddObject(ptr);
		break;
	}
	case SceneType::Gameplay: {
		FOV = 3840;
		auto sunPtr = std::make_shared<PlanetSun>(canvas);
		AddObject(sunPtr);
		{
			auto ptr = std::make_shared<PlanetEarth>(canvas);
			ptr->SetParentPlanet(sunPtr);
			SetCameraTarget(ptr.get());
			AddObject(ptr);
		}
		{
			auto ptr = std::make_shared<PlanetJupiter>(canvas);
			ptr->SetParentPlanet(sunPtr);
			AddObject(ptr);
		}
		{
			auto ptr = std::make_shared<PlanetMars>(canvas);
			ptr->SetParentPlanet(sunPtr);
			AddObject(ptr);
		}
		{
			auto ptr = std::make_shared<PlanetNeptune>(canvas);
			ptr->SetParentPlanet(sunPtr);
			AddObject(ptr);
		}
		break;
	}
	case SceneType::HesitateEnd: {
		auto ptr = std::make_shared<BaseEnd>(canvas, nextScene);
		SetCameraTarget(nullptr);
		camera.globalPosition = { 0,0 };
		FOV = 720;
		AddObject(ptr);
		break;
	}
	case SceneType::IntoTheSunEnd: {
		auto ptr = std::make_shared<BaseEnd>(canvas, nextScene);
		SetCameraTarget(nullptr);
		camera.globalPosition = { 0,0 };
		FOV = 720;
		AddObject(ptr);
		break;
	}
	case SceneType::JupiterEnd: {
		auto ptr = std::make_shared<BaseEnd>(canvas, nextScene);
		SetCameraTarget(nullptr);
		camera.globalPosition = { 0,0 };
		FOV = 720;
		AddObject(ptr);
		break;
	}
	case SceneType::MarsEnd: {
		auto ptr = std::make_shared<BaseEnd>(canvas, nextScene);
		SetCameraTarget(nullptr);
		camera.globalPosition = { 0,0 };
		FOV = 720;
		AddObject(ptr);
		break;
	}
	case SceneType::NeptuneEnd: {
		auto ptr = std::make_shared<BaseEnd>(canvas, nextScene);
		SetCameraTarget(nullptr);
		camera.globalPosition = { 0,0 };
		FOV = 720;
		AddObject(ptr);
		break;
	}
	case SceneType::TheEyeEnd: {
		auto ptr = std::make_shared<BaseEnd>(canvas, nextScene);
		SetCameraTarget(nullptr);
		camera.globalPosition = { 0,0 };
		FOV = 720;
		AddObject(ptr);
		break;
	}
	default:
		break;
	}
}

void Scene::SetCameraTarget(BaseGameObject* target) {
	camera.SetFollowTarget(target);
}

BaseGameObject* Scene::GetCameraTarget() {
	return camera.followTarget;
}

Vector2 Scene::GetCameraTargetPos() {
	if (camera.followTarget) {
		return camera.followTarget->globalPosition;
	}
	else {
		return camera.globalPosition;
	}
}

void Scene::SetCameraBoundary(const AABB& newBound) {
	camera.SetBoundary(newBound);
}

AABB Scene::GetCameraAABB() {
	return camera.GetAABB();
}

Vector2 Scene::GetGravityOn(BasePhysicalObject* target)
{
	Vector2 forceGravity(0, 0);
	if (target->isPendingDestroy || !target->isActive) {
		return forceGravity;
	}
	if (!target || target->isStatic) {
		return forceGravity;
	}

	for (int i = gravitySourceObjects.size() - 1; i >= 0; --i) {
		auto gravityPtr = gravitySourceObjects[i].lock();
		if (!gravityPtr) {
			gravitySourceObjects.erase(gravitySourceObjects.begin() + i);
			continue;
		}
		if (target == gravityPtr.get()) {
			continue;
		}
		Vector2 dir = gravityPtr->globalPosition - target->globalPosition;
		float len = dir.Length();
		if (len < 1e-4f) {
			continue;
		}
		forceGravity = forceGravity + dir.Normalized() * GRAVITY_CONSTANT * target->mass * gravityPtr->mass / (len * len);
	}

	return forceGravity;
}

void Scene::RenderWorker(const float& alpha, std::atomic<int>& nextTile, std::vector<RenderedItem>& renderedItems) {
	int tile;
	while ((tile = nextTile.fetch_add(1, std::memory_order_relaxed)) < renderTiles.size()) {
		bool isRendered;
		short depth = 0;
		// depth test
		for (int order = 3; order >= 0; --order) {
			for (int i = objects.size() - 1; i >= 0; --i) {
				if (!objects[i]->isActive || objects[i]->isPendingDestroy) {
					continue;
				}
				if (objects[i]->renderOrder == order) {
					isRendered = objects[i]->DepthTest(alpha, camera, depth, renderTiles[tile]);
					if (isRendered) {
						renderedItems.push_back({ objects[i].get() , depth });
						++depth;
					}
				}
			}
		}

		for (int i = renderedItems.size() - 1; i >= 0; --i) {
			renderedItems[i].objPtr->RenderSelf(alpha, camera, renderedItems[i].depth, renderTiles[tile]);
		}

		renderedItems.clear();
	}
}

//void Scene::RenderWorkerNoDepth(const float& alpha, std::atomic<int>& nextTile,
//	std::vector<RenderedItem>& renderedItems) {
//	int tile;
//	while ((tile = nextTile.fetch_add(1, std::memory_order_relaxed)) < renderTiles.size()) {
//		for (int order = 0; order <= 3; ++order) {
//			for (int i = 0; i < objects.size(); ++i) {
//				if (!objects[i]->isActive || objects[i]->isPendingDestroy) {
//					continue;
//				}
//				if (objects[i]->renderOrder == order) {
//					objects[i]->RenderSelf(alpha, camera, -1, renderTiles[tile]);
//				}
//			}
//		}
//	}
//}