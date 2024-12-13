//
// Created by Alfie on 13/12/2024.
//

#include "WinScreen.h"

#include "Debug.h"
#include "ScreenManager.h"
#include "Window.h"
#include "../Game.h"

void WinScreen::OnFirstLoad() {
    NCL::Debug::Print("GAME CLEARED! WELL DONE", NCL::Vector2(5, 85));
    NCL::Debug::Print("Press ENTER to restart!", NCL::Vector2(5, 75));
}

void WinScreen::Update(float dt) {
    if (NCL::Window::GetWindow()->GetKeyboard()->KeyPressed(NCL::KeyCodes::RETURN)) {
        ScreenManager::Pop();
        dynamic_cast<Game*>(ScreenManager::GetCurrent())->Reset();
    }
}

