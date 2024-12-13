//
// Created by Alfie on 13/12/2024.
//

#ifndef DEATHSCREEN_H
#define DEATHSCREEN_H
#include "Screen.h"


class DeathScreen : public Screen {
public:
    void OnFirstLoad() override;
    void Update(float dt) override;
};



#endif //DEATHSCREEN_H
