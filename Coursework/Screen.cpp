//
// Created by c1012538 on 03/12/2024.
//

#include "PushdownState.h"
#include "Window.h"

using namespace NCL;

namespace Screen {
    class GameScreen : public CSC8503::PushdownState {

    };

    class IntroScreen : public CSC8503::PushdownState {
        PushdownResult OnUpdate(float dt, PushdownState **newState) override {
            if (Window::GetKeyboard()->KeyPressed(KeyCodes::RETURN)) {
                *newState = new GameScreen();
                return Push;
            }

            if (Window::GetKeyboard()->KeyDown(KeyCodes::ESCAPE)) {
                return Pop;
            }

            return NoChange;
        }
    };
}
