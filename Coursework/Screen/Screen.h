//
// Created by Alfie on 10/12/2024.
//

#ifndef SCREEN_H
#define SCREEN_H



class Screen {
public:
    //virtual ~Screen() = 0;
    virtual void OnFirstLoad() = 0;
    virtual void Update(float dt) = 0;
};



#endif //SCREEN_H
