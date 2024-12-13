//
// Created by Alfie on 12/12/2024.
//

#ifndef CHARACTER_H
#define CHARACTER_H
#include "GameObject.h"

class Character : public NCL::CSC8503::GameObject {
public:
    virtual void Update(float dt) = 0;
    [[nodiscard]] Vector3 GetStartPos() const { return startPos; }

protected:
    Vector3 startPos = Vector3();
};

#endif //CHARACTER_H
