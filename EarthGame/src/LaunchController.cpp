#include"LaunchController.h"
#include"Camera.h"
#include"../TestObject.h"
#include<iostream>

LaunchController::LaunchController(GamesEngineeringBase::Window& win, Camera& cam, TestObject& target) :canvas(win), camera(cam), earth(target)
{
	launchPoint = earth.globalPosition;
}

void LaunchController::UpdateInput() {
	bool isMouseLeft = canvas.mouseButtonPressed(GamesEngineeringBase::MouseLeft);//state judgement
	bool isReset = canvas.keyPressed('R');
	if (isReset && !wasReset) {
		Reset();
	}
	wasReset = isReset;
	if (isMouseLeft && !leftMouseDown && state == LaunchState::Ready) {
		state = LaunchState::Aiming;

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
	Vector2 mouseWorld = camera.GetGlobalPosition(mouseX, mouseY);//get mouse position
	mouseDirection = launchPoint - mouseWorld;
	float distance = mouseDirection.Length();//get the distance
	if (distance > maxDragDistance) {
		mouseDirection = mouseDirection.Normalized()*maxDragDistance;
	}

}

void LaunchController::Release() {
	Vector2 launchImpulse;
	launchImpulse = mouseDirection * launchStrength;//J = d*k
	earth.SetLauncherLocked(false);
	std::cout << "Launch X: " << launchImpulse.x << " Y: " << launchImpulse.y << std::endl;
	earth.ApplyLaunchImpulse(launchImpulse);
	state = LaunchState::Flying;
}

void LaunchController::Reset() {
	earth.ResetForLaunch(launchPoint);
	mouseDirection = Vector2();
	state = LaunchState::Ready;
}