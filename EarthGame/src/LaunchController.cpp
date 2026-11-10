#include"LaunchController.h"
#include"Camera.h"
#include"../TestObject.h"

LaunchController::LaunchController(GamesEngineeringBase::Window& win, BasePlanet* target)
	: canvas(win), earth(target) {
	launchPoint = {};
	leftMouseDown = canvas.mouseButtonPressed(GamesEngineeringBase::MouseLeft);
	launchStrength = 2600.f;
}

void LaunchController::UpdateInput() {
	bool isMouseLeft = canvas.mouseButtonPressed(GamesEngineeringBase::MouseLeft);//state judgement
	if (isMouseLeft && !leftMouseDown && state == LaunchState::Ready) {
		state = LaunchState::Aiming;
		launchPoint = { (float)canvas.getMouseInWindowX(), (float)canvas.getMouseInWindowY() }; 
		startScale = earth->scale;
	}
	else if (isMouseLeft && leftMouseDown && state == LaunchState::Aiming) {
		state = LaunchState::Aiming;
		UpdateAiming();//update the direction it will face
	}
	else if (!isMouseLeft && leftMouseDown && state == LaunchState::Aiming ) {
		Release();
	}
	leftMouseDown = isMouseLeft;
}

void LaunchController::UpdateAiming() {
	float mouseX = canvas.getMouseInWindowX();
	float mouseY = canvas.getMouseInWindowY();
	mouseDirection = launchPoint - Vector2(mouseX, mouseY);
	float distance = mouseDirection.Length();//get the distance
	distance = std::clamp(distance, minDragDistance, maxDragDistance);
	mouseDirection = mouseDirection.Normalized() * distance;

	earth->rotation = std::atan2(mouseDirection.y, mouseDirection.x);
	float alpha = (mouseDirection.Length() - minDragDistance)
		/ (maxDragDistance - minDragDistance);
	earth->scale.x = startScale.x * (1 + alpha);
	earth->scale.y = startScale.x / (1 + alpha);
}

void LaunchController::Release() {
	Vector2 launchImpulse;
	launchImpulse = mouseDirection.Normalized() * launchStrength;//J = d*k
	earth->AddForce(launchImpulse);
	state = LaunchState::Flying;
	earth->scale = startScale;
	earth->UpdateStates();
	earth->OnRelease();
}
