//
// Created by Alfie on 13/12/2024.
//

#ifndef ITEM_H
#define ITEM_H

class Item {
public:
    explicit Item(short int const id) { this->id = id; }

    [[nodiscard]] short int GetId() const { return id; }

protected:
    short int id;
};

#endif //ITEM_H
