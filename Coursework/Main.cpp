//
// Created by c1012538 on 03/12/2024.
//

#include "Game.h"
#include "Window.h"
#include "Screen/ScreenManager.h"
#include "Screen/StartScreen.h"

using namespace NCL;
using namespace CSC8503;

int main() {
    WindowInitialisation initInfo;
    initInfo.width = 1280;
    initInfo.height = 720;
    initInfo.windowTitle = "One Cute Tabby, Not Too Shabby!";

    Window *window = Window::CreateGameWindow(initInfo);
    if (!window->HasInitialised()) return -1;

    window->GetTimer().GetTimeDeltaSeconds();

    auto *game = new Game();
    ScreenManager::Append(game);

    auto startScreen = StartScreen();
    StartScreen* s = &startScreen;
    ScreenManager::Append(s);

    game->Update(window->GetTimer().GetTimeDeltaSeconds());

    while (window->UpdateWindow() && !Window::GetKeyboard()->KeyDown(KeyCodes::ESCAPE)) {
        float const dt = Window::GetTimer().GetTimeDeltaSeconds();

        if (dt > 0.1f) {
            std::cout << "Skipping large time delta" << std::endl;
            continue;
        }

        window->SetTitle("One Cute Tabby, Not Too Shabby! Frame time: " + std::to_string(1000.0f * dt));

        ScreenManager::GetCurrent()->Update(dt);
    }
    Window::DestroyGameWindow();

    return 0;
}
