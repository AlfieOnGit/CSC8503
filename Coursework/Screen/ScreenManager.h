//
// Created by Alfie on 13/12/2024.
//

#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H
#define MAX_HEIGHT 3
#include "Screen.h"

class ScreenManager final {
public:
    //static void SetCurrent(Screen* screen) { current = screen; }
    //static Screen* GetCurrent() { return current; }
    static void Append(Screen* screen);
    static void Pop();
    static Screen* GetCurrent();

protected:
    static Screen* screen_stack[MAX_HEIGHT];
    static short int height;
    //static Screen* current;
};

#endif //SCREENMANAGER_H
