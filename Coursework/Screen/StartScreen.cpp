//
// Created by Alfie on 13/12/2024.
//

#include "StartScreen.h"

#include "Debug.h"
#include "ScreenManager.h"
#include "Window.h"

void StartScreen::OnFirstLoad() {
    NCL::Debug::Print("Press SPACE to start!", NCL::Vector2(30, 50));
    NCL::Window::GetWindow()->ShowOSPointer(true);
    NCL::Window::GetWindow()->LockMouseToWindow(false);
}

void StartScreen::Update(float dt) {
    if (NCL::Window::GetKeyboard()->KeyPressed(NCL::KeyCodes::SPACE)) {
        ScreenManager::Pop();
    }
}

