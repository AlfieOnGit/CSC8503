//
// Created by Alfie on 13/12/2024.
//

#include "DeathScreen.h"

#include "Debug.h"
#include "ScreenManager.h"
#include "Window.h"
#include "../Game.h"

void DeathScreen::OnFirstLoad() {
    NCL::Debug::Print("You died! Press ENTER to restart!", NCL::Vector2(5, 85));
    Window::GetWindow()->ShowOSPointer(true);
    Window::GetWindow()->LockMouseToWindow(false);
}


void DeathScreen::Update(float dt) {
    if (NCL::Window::GetKeyboard()->KeyPressed(NCL::KeyCodes::RETURN)) {
        ScreenManager::Pop();
        dynamic_cast<Game*>(ScreenManager::GetCurrent())->Reset();
    }
}
