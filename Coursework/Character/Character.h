﻿//
// Created by Alfie on 12/12/2024.
//

#ifndef CHARACTER_H
#define CHARACTER_H
#include "GameObject.h"

class Character : public NCL::CSC8503::GameObject {
public:
    virtual void Update(float dt) = 0;
};

#endif //CHARACTER_H
