//
// Created by Alfie on 13/12/2024.
//

#include "ScreenManager.h"

//Screen* ScreenManager::current[3];
Screen* ScreenManager::screen_stack[3];
short int ScreenManager::height = 0;

void ScreenManager::Append(Screen *screen) {
    if (height == MAX_HEIGHT) return;
    screen_stack[height++] = screen;
    screen->OnFirstLoad();
}

void ScreenManager::Pop() {
    if (height == 0) return;
    screen_stack[--height] = nullptr;
}

Screen *ScreenManager::GetCurrent() {
    if (height == 0) return nullptr;
    return screen_stack[height - 1];
}

