//
// Created by c1012538 on 03/12/2024.
//

#include "PushdownMachine.h"
#include "Window.h"

using namespace NCL;
using namespace CSC8503;

int main() {
  WindowInitialisation initInfo;
  initInfo.width		= 1280;
  initInfo.height		= 720;
  initInfo.windowTitle = "One Cute Tabby, Not Too Shabby!";

  Window* window = Window::CreateGameWindow(initInfo);
  if (!window->HasInitialised()) return -1;

  auto* pdm = new PushdownMachine()

  return 0;
}
