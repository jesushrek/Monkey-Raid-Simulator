#ifndef DOG_HPP
#define DOG_HPP

#include "Creatures.hpp"

class Dog:public Creatures
{ 
    public:
        //dogs do not do damage they only scare the monkeys away and have 4 health
        Dog(int id)
            :Creatures(Creatures::Dog, id, 'd', 4, 0 )
        {}
};
#endif
