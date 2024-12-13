//
// Created by c1012538 on 03/12/2024.
//

#include "Game.h"
#include "Window.h"

using namespace NCL;
using namespace CSC8503;

int main() {
    WindowInitialisation initInfo;
    initInfo.width = 1280;
    initInfo.height = 720;
    initInfo.windowTitle = "One Cute Tabby, Not Too Shabby!";

    Window *window = Window::CreateGameWindow(initInfo);
    if (!window->HasInitialised()) return -1;

    window->ShowOSPointer(false);
    window->LockMouseToWindow(true);

    auto *game = new Game();

    window->GetTimer().GetTimeDeltaSeconds();

    while (window->UpdateWindow() && !Window::GetKeyboard()->KeyDown(KeyCodes::ESCAPE)) {
        float const dt = Window::GetTimer().GetTimeDeltaSeconds();

        if (dt > 0.1f) {
            std::cout << "Skipping large time delta" << std::endl;
            continue;
        }

        window->SetTitle("One Cute Tabby, Not Too Shabby! Frame time: " + std::to_string(1000.0f * dt));

        game->Update(dt);
    }
    Window::DestroyGameWindow();

    return 0;
}
