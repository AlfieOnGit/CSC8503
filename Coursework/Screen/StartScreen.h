//
// Created by Alfie on 13/12/2024.
//

#ifndef STARTSCREEN_H
#define STARTSCREEN_H
#include "Screen.h"


class StartScreen : public Screen {
public:
    void OnFirstLoad() override;
    void Update(float dt) override;
};



#endif //STARTSCREEN_H
