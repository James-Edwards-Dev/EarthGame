#pragma once
#include "BaseGameObject.h"
#include "../GamesEngineeringBase.h"
#include <vector>
#include "Camera.h"
#include <memory>
#include "SmallStructs.h"
#include <atomic>
#include "../Constants.h"

// a scene contains lots of objects, and manage their life circles
class Scene{
protected:
	std::vector<std::shared_ptr<BaseGameObject>> objects;
	std::vector<std::weak_ptr<BasePhysicalObject>> gravitySourceObjects;
	//std::vector<std::shared_ptr<ClickableText>> uiObjects;
	GamesEngineeringBase::Window& canvas;
	Camera& camera;
	std::vector<ScreenAABB> renderTiles;
	std::vector<std::vector<RenderedItem>> renderedItemLists{ CO_RENDER_THREADS + 1 };
	 
public:

	Scene(GamesEngineeringBase::Window& window, Camera& camera);

	virtual ~Scene() {
		for (int i = 0; i < objects.size(); ++i) {
			objects[i]->OnDestroy();
		}
	}

	void AddObject(std::shared_ptr<BaseGameObject> obj);
	//void AddUIObject(std::shared_ptr<ClickableText> uiObject);

	virtual void FixedUpdateObjects(const float& dt);
	virtual void VisualUpdateObjects(const float& dt);
	virtual void RenderObjects(const float& alpha);
	virtual void RecycleObjects();
	// when quit, destroy everything to ensure OnDestroy is fully triggered
	void RecycleAllObjects();

	void SetCameraTarget(BaseGameObject* target);
	BaseGameObject* GetCameraTarget();
	Vector2 GetCameraTargetPos();
	void SetCameraBoundary(const AABB& newBound);
	AABB GetCameraAABB();
	Vector2 GetGravityOn(BasePhysicalObject* target);

private:
	void RenderWorker(const float& alpha, std::atomic<int>& nextTile, std::vector<RenderedItem>& renderedItems);
	//void RenderWorkerNoDepth(const float& alpha, std::atomic<int>& nextTile, std::vector<RenderedItem>& renderedItems);
};