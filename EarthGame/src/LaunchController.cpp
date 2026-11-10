#include"LaunchController.h"
#include"Camera.h"
#include"../TestObject.h"
#include<iostream>

LaunchController::LaunchController(GamesEngineeringBase::Window& win, BasePhysicalObject* target)
	: canvas(win), earth(target) {
	launchPoint = {};
	leftMouseDown = canvas.mouseButtonPressed(GamesEngineeringBase::MouseLeft);
}

void LaunchController::UpdateInput() {
	bool isMouseLeft = canvas.mouseButtonPressed(GamesEngineeringBase::MouseLeft);//state judgement
	if (isMouseLeft && !leftMouseDown && state == LaunchState::Ready) {
		state = LaunchState::Aiming;
		launchPoint = { (float)canvas.getMouseInWindowX(), (float)canvas.getMouseInWindowY() };
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
}

void LaunchController::Release() {
	Vector2 launchImpulse;
	launchImpulse = mouseDirection * launchStrength;//J = d*k
	std::cout << "Launch X: " << launchImpulse.x << " Y: " << launchImpulse.y << std::endl;
	earth->AddForce(launchImpulse);
	state = LaunchState::Flying;
}
