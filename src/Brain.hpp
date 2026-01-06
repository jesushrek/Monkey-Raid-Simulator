#include "Constants.hpp"
#include "Creatures.hpp"
#include "Field.hpp"
#include "Crops.hpp"
#include "House.hpp"
#include "Dog.hpp"

#include <cmath>
Creatures::Direction Creatures::bestDirection(Creatures::Point to, Field<Config::HEIGHT, Config::WIDTH>& field)
{ 
    Creatures::Direction choice[] { Creatures::Up, Creatures::Down, Creatures::Left, Creatures::Right };
    Creatures::Direction bestDirection {};

    int minDistance {1000};
    for(int i = 0; i < Creatures::MaxDirections; ++i)
    { 
        Creatures::Point next = getPoint();
        Creatures::Direction testDirection{choice[i]};

        if(testDirection == Creatures::Up) 
            next.y = (next.y - 1 + Config::HEIGHT) % Config::HEIGHT;
        if(testDirection == Creatures::Down) 
            next.y = (next.y + 1) % Config::HEIGHT;
        if(testDirection == Creatures::Right)
            next.x = (next.x + 1) % Config::WIDTH;
        if(testDirection == Creatures::Left)
            next.x = (next.x - 1 + Config::WIDTH) % Config::WIDTH;

        // If something is infront avoid him
        if(field.getGrid()[next.y * Config::WIDTH + next.x] != nullptr)
            continue;

        int distance = std::abs(next.x - to.x) + std::abs(next.y - to.y);
        if(distance < minDistance)
        { 
            minDistance = distance;
            bestDirection = testDirection;
        }
    }

    return bestDirection;
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
    int minDistance{ 1000 };
    // find the nearest Monkey and move towards it
    Creatures* closetMonkey = nullptr;
    for (auto& creature : field.getGrid())
    { 
        if(creature != nullptr)
        { 
            if (creature->getType() == Creatures::Monkey)
            { 
                int distance{};
                distance = std::abs(creature->getPoint().x - getPoint().x)
                    + std::abs(creature->getPoint().y - getPoint().y);

                if(distance < minDistance)
                { 
                    minDistance = distance;
                    closetMonkey = creature;
                }
            }
        }
    }

    if(!closetMonkey)
    { 
        setState(Idle);
        return;
    }
}
