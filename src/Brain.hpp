#include "Creatures.hpp"
#include "Field.hpp"
#include "Constants.hpp"

class Crops;
class House;
class Dog;
class Monkey;
class Human;

Creatures::Point directionToPoint(Creatures::Direction dir)
{ 
    switch(dir)
    { 
        case Creatures::Up: return { -1, 0 };
        case Creatures::Down: return { 1, 0 };
        case Creatures::Left: return { 0, -1 };
        case Creatures::Right: return { 0, 1 };
        default: break;
    }
    return { 0, 0 };
}

void Crops::Ai(Field<Config::HEIGHT, Config::WIDTH>& field)
{ 
    return;
}

void House::Ai(Field<Config::HEIGHT, Config::WIDTH>& field)
{ 
    return;
}

void Dog::Ai(Field<Config::HEIGHT, Config::WIDTH>& field)
{ 
    // find the nearest Monkey and move towards it
    for (auto& creature : field.getGrid())
    { 
        if (creature->getType() == Creatures::Monkey)
        { 
            int chance = rand() % 100;
            if(chance < 70) // 70% chance to move towards the monkey;
            { 
            }
        }
    }
}
