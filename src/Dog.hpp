#ifndef DOG_HPP
#define DOG_HPP

#include "Creatures.hpp"
#include "Constants.hpp"

template<int H, int W>
class Field;

class Dog:public Creatures
{ 
    public:
        //dogs do not do damage they only scare the monkeys away and have 4 health
        Dog(int x, int y, int id)
            :Creatures(Point{x, y}, Creatures::Dog, id, 'd', 4, 0 )
        {}

        virtual void Ai(Field<Config::HEIGHT, Config::WIDTH>& field) override;
};
#endif
