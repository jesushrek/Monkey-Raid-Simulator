#ifndef BRAIN_HPP
#define BRAIN_HPP

#include "Constants.hpp"
#include "Creatures.hpp"
#include "Field.hpp"
#include "Crops.hpp"
#include "House.hpp"
#include "Dog.hpp"
#include "Monkey.hpp"
#include "Human.hpp"

#include <cmath>

constexpr Creatures::Direction Stop = Creatures::MaxDirections;

Creatures::Point returnPoint(Creatures::Direction dir, Creatures::Point p)
{ 
    Creatures::Point next{p};
    if(dir == Creatures::Up) 
        next.y = (next.y - 1);
    if(dir == Creatures::Down) 
        next.y = (next.y + 1); 
    if(dir == Creatures::Right)
        next.x = (next.x + 1);
    if(dir == Creatures::Left)
        next.x = (next.x - 1);
    if(dir == Creatures::Upleft)
    {
        next.x = next.x - 1;
        next.y = next.y - 1;
    }
    if(dir == Creatures::Upright)
    { 
        next.x = next.x + 1;
        next.y = next.y - 1;
    }
    if(dir == Creatures::Downleft)
    { 
        next.x = next.x - 1;
        next.y = next.y + 1;
    }
    if(dir == Creatures::Downright)
    {
        next.x = next.x + 1;
        next.y = next.y + 1;
    }

    if(dir == Stop)
        return p;

    if(next.y < 0 || next.y >= Config::HEIGHT || next.x < 0 || next.x >= Config::WIDTH)
        return p;

    return next;
}

Creatures::Direction Creatures::fleeDirection(Creatures::Point from, Field<Config::HEIGHT, Config::WIDTH>& field)
{ 
    Direction choice[] { Up, Down, Left, Right, Upright, Upleft, Downright, Downleft };
    Direction bestDirection {};

    int maxDistance {-1000};
    bool foundBest { false };
    for(int i = 0; i < MaxDirections; ++i)
    { 
        Point next = getPoint();
        Direction testDirection{choice[i]};

        next = returnPoint(testDirection, next);
        // If something is infront avoid him
        if(field.getGrid()[next.y * Config::WIDTH + next.x] != nullptr)
            continue;


        int distance{ std::max(std::abs(next.x - from.x), std::abs(next.y - from.y)) };
        if(distance > maxDistance)
        { 
            foundBest = true;
            maxDistance = distance;
            bestDirection = testDirection;
        }
    }

    if(!foundBest)
        return Stop;

    return bestDirection;
}

Creatures::Direction Creatures::bestDirection(Creatures::Point to, Field<Config::HEIGHT, Config::WIDTH>& field)
{ 
    Direction choice[] { Up, Down, Left, Right, Upright, Upleft, Downright, Downleft };
    Direction bdir {};

    int minDistance {1000};
    bool foundBest { false };
    for(int i = 0; i < MaxDirections; ++i)
    { 
        Point next = getPoint();
        Direction testDirection{choice[i]};

        next = returnPoint(testDirection, next);
        // If something is infront avoid him
        if(field.getGrid()[next.y * Config::WIDTH + next.x] != nullptr)
            continue;

        int distance {std::max(std::abs(next.x - to.x), std::abs(next.y - to.y))};
        if(distance < minDistance)
        { 
            foundBest = true;
            minDistance = distance;
            bdir = testDirection;
        }
    }

    if(!foundBest)
        return Stop;

    return bdir;
}

void Crops::Ai(Field<Config::HEIGHT, Config::WIDTH>& field)
{ 
    setMoved(true);
    return;
}

void House::Ai(Field<Config::HEIGHT, Config::WIDTH>& field)
{ 
    setMoved(true);
    return;
}

void Creatures::move(Direction dir, Field<Config::HEIGHT, Config::WIDTH>& field)
{
    setMoved(true);
    if(dir == Stop)
    { 
        return;
    } 
    Point oldPoint { getPoint() };
    Point newPoint { returnPoint(dir, oldPoint) };

    if(newPoint.x == oldPoint.x && newPoint.y == oldPoint.y)
    {
        return;
    }

    //if something is on the way avoid him
    if (field.getGrid()[newPoint.y * Config::WIDTH + newPoint.x] != nullptr) 
    {
        return;
    }


    setPoint(newPoint);
    int oldIndex = oldPoint.y * Config::WIDTH + oldPoint.x;
    int newIndex = newPoint.y * Config::WIDTH + newPoint.x;
    field.getGrid()[newIndex] = field.getGrid()[oldIndex];
    field.getGrid()[oldIndex] = nullptr;
}

Creatures* Creatures::returnCloset(Field<Config::HEIGHT, Config::WIDTH>& field, Type target)
{ 
    int minDistance{ 1000 };

    Creatures* closetTarget{ nullptr };
    for(auto& creature : field.getGrid())
    { 
        if(creature != nullptr)
        { 
            if(creature->getType() == target)
            { 
                int distance { 
                    std::max(std::abs(creature->getPoint().x - getPoint().x), std::abs(creature->getPoint().y - getPoint().y)) 
                };

                if(distance < minDistance)
                { 
                    minDistance = distance;
                    closetTarget = creature;
                }
            }
        }
    }
    if(!closetTarget)
        return nullptr;

    return closetTarget;
}

void Dog::Ai(Field<Config::HEIGHT, Config::WIDTH>& field)
{ 
    // find the nearest Monkey and move towards it
    Creatures* closetMonkey = returnCloset(field, Type::Monkey);
    if(!closetMonkey)
    { 
        setState(Idle);
        setMoved(true);
        return;
    }
    setState(Moving);
    Creatures::Direction direction = bestDirection(closetMonkey->getPoint(), field);
    move(direction, field);
}

void Monkey::Ai(Field<Config::HEIGHT, Config::WIDTH>& field)
{ 
    //how the monkey deals with his neighbors

    int countHumans{};
    bool dogsNearby { false };
    bool houseNearby { false };
    bool cropsNearby { false };

    Creatures* immediateTarget{ nullptr };
    Creatures* threat{ nullptr };
    for(int h = -1; h <= 1; ++h)
    { 
        for(int w = -1; w <= 1; ++w)
        { 
            Creatures::Point monkeyPoint{ getPoint() };

            if((w == 0 && h == 0)
                    || ((h + monkeyPoint.y) >= Config::HEIGHT)
                    || ((w + monkeyPoint.x) >= Config::WIDTH) 
                    || ((h + monkeyPoint.y) < 0)
                    || ((w + monkeyPoint.x) < 0)
              )
                continue;

            int y = (h + monkeyPoint.y);
            int x = (w + monkeyPoint.x);


            Creatures* neighbor { field.getGrid()[y * Config::WIDTH + x] }; 

            if(neighbor)
                countHumans += (neighbor->getType() == Creatures::Human)? 1 : 0;

            if(neighbor == nullptr)
                continue;
            // if there are more then 2 humans as neighbors just run
            else if(neighbor->getType() == Creatures::Dog)
                dogsNearby = true;

            else if(neighbor->getType() == Creatures::Human)
            { 
                threat = neighbor;
                ++countHumans;
            }

            else if(neighbor->getType() == Creatures::House)
            {
                immediateTarget = neighbor;
                houseNearby = true;
            }

            else if(neighbor->getType() == Creatures::Crops)
            {
                immediateTarget = neighbor;
                cropsNearby = true;
            }
        }
    }

    if(countHumans > 2 || dogsNearby)
    { 
        if(threat) 
        { 
            Creatures::Direction bestDir { fleeDirection(threat->getPoint(), field) };
            move(bestDir, field);
            setState(Moving);
            setMoved(true);
            return;
        }
    }
    else if(houseNearby)
    { 
        if(immediateTarget) { 
            attack(*immediateTarget);
            setMoved(true);
            return;
        }
    }
    else if(cropsNearby)
    { 
        if(rand() % 4)
        {
        if(immediateTarget) 
        { 
            attack(*immediateTarget);
            setMoved(true);
            return;
        }
        }
    }
    else
    {
        if(rand() % 2)
        { 
            if(threat) 
            { 
                Creatures::Direction bestDir { fleeDirection(threat->getPoint(), field) };
                move(bestDir, field);
                setState(Moving);
                setMoved(true);
                return;
            }
        }
        else
        { 
            if(threat)
            {
                attack(*threat);
                setMoved(true);
                return;
            }
        }

    }
    /*
       Creatures::Type targets[] { 
       Creatures::Crops, 
       Creatures::House, 
       Creatures::Human
       };
       */

    //implement grazing/attacking behaviour
    Creatures* closetTarget{ nullptr };

    int minDistance = 1000;
    for(int i = 0; i < Config::HEIGHT; ++i)
    { 
        for(int j = 0; j < Config::WIDTH; ++j)
        { 
            Creatures* target = field.getGrid()[i * Config::WIDTH + j];
            if(target == nullptr) continue;

            if(target->getType() == Creatures::Dog || target->getType() == Creatures::Monkey) continue;

            int distance { std::max(std::abs(target->getPoint().x - getPoint().x) , std::abs(target->getPoint().y - getPoint().y))
            };

            if(distance < minDistance)
            { 
                minDistance = distance;
                closetTarget = target;
            }
        }
    }

    if(!closetTarget)
    { 
        setState(Idle);
        move(Stop, field);
        setMoved(true);
        return;
    }

    Creatures::Direction bestDir = bestDirection(closetTarget->getPoint(), field);
    move(bestDir, field);
}

void Human::Ai(Field<Config::HEIGHT, Config::WIDTH>& field)
{ 
    int countMonkeys{};
    Creatures* immediateTarget { nullptr };
    for(int h = -1; h <= 1; ++h)
    { 
        for(int w = -1; w <= 1; ++w)
        { 
            Creatures::Point humanPoint{ getPoint() };

            if((w == 0 && h == 0)
                    || ((h + humanPoint.y) >= Config::HEIGHT)
                    || ((w + humanPoint.x) >= Config::WIDTH) 
                    || ((h + humanPoint.y) < 0)
                    || ((w + humanPoint.x) < 0)
              )
                continue;

            int y = (h + humanPoint.y);
            int x = (w + humanPoint.x);

            Creatures* neighbor { field.getGrid()[y * Config::WIDTH + x] }; 
            if(neighbor == nullptr) continue;

            if(neighbor->getType() == Creatures::Monkey)
            { 
                immediateTarget = neighbor;
                ++countMonkeys;
            }
        }
    }

    if(countMonkeys > 2)
    {
        if(immediateTarget)
        { 
            Creatures::Direction bestDir { fleeDirection(immediateTarget->getPoint(), field) };
            move(bestDir, field);
            setState(Moving);
            setMoved(true);
            return;
        }

    }
    else
    {
        if(rand() % 2)
        {
            if(immediateTarget) 
            { 
                attack(*immediateTarget);
                setMoved(true);
                return;
            }
        }
    }

    Creatures* closetMonkey { returnCloset(field, Creatures::Monkey) };
    if(closetMonkey == nullptr)
    { 
        setState(Idle);
        setMoved(true);
        move(Stop, field);
        return;
    }

    if(closetMonkey)
    {
        Creatures::Direction bestDir { bestDirection(closetMonkey->getPoint(), field) };
        move(bestDir, field);
    }
    setMoved(true);
}

#endif
