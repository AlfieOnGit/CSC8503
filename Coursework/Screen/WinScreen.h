//
// Created by Alfie on 13/12/2024.
//

#ifndef WINSCREEN_H
#define WINSCREEN_H
#include "Screen.h"


class WinScreen : public Screen {
public:
    void OnFirstLoad() override;
    void Update(float dt) override;
};



#endif //WINSCREEN_H
