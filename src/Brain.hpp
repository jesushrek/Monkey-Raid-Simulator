#ifndef BRAIN_HPP
#define BRAIN_HPP

#include "Constants.hpp"
#include "Creatures.hpp"
#include "Field.hpp"
#include "Crops.hpp"
#include "House.hpp"
#include "Dog.hpp"
#include "Monkey.hpp"

#include <cmath>

constexpr Creatures::Direction Stop = Creatures::MaxDirections;

Creatures::Point returnPoint(Creatures::Direction dir, Creatures::Point p)
{ 
    Creatures::Point next{p};
    if(dir == Creatures::Up) 
        next.y = (next.y - 1 + Config::HEIGHT);
    if(dir == Creatures::Down) 
        next.y = (next.y + 1); 
    if(dir == Creatures::Right)
        next.x = (next.x + 1);
    if(dir == Creatures::Left)
        next.x = (next.x - 1 + Config::WIDTH);
    if(dir == Stop)
        return p;

    return next;
}

Creatures::Direction Creatures::fleeDirection(Creatures::Point from, Field<Config::HEIGHT, Config::WIDTH>& field)
{ 
    Direction choice[] { Up, Down, Left, Right };
    Direction bestDirection {};

    int maxDistance {-100};
    bool foundBest { false };
    for(int i = 0; i < MaxDirections; ++i)
    { 
        Point next = getPoint();
        Direction testDirection{choice[i]};

        next = returnPoint(testDirection, next);
        // If something is infront avoid him
        if(field.getGrid()[next.y * Config::WIDTH + next.x] != nullptr)
            continue;

        int distance = std::abs(next.x - from.x) + std::abs(next.y - from.y);
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
    Direction choice[] { Up, Down, Left, Right };
    Direction bestDirection {};

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

        int distance = std::abs(next.x - to.x) + std::abs(next.y - to.y);
        if(distance < minDistance)
        { 
            foundBest = true;
            minDistance = distance;
            bestDirection = testDirection;
        }
    }

    if(!foundBest)
        return Stop;

    return bestDirection;
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
    if(dir == Stop)
    { 
        setMoved(true);
        return;
    } 
    Point oldPoint { getPoint() };
    Point newPoint { returnPoint(dir, oldPoint) };

    //if something is on the way avoid him
    if (field.getGrid()[newPoint.y * Config::WIDTH + newPoint.x] != nullptr) 
        return;


    setPoint(newPoint);
    setMoved(true);

    field.getGrid()[newPoint.y * Config::WIDTH + newPoint.x] = field.getGrid()[oldPoint.y * Config::WIDTH + oldPoint.x];
    field.getGrid()[oldPoint.y * Config::WIDTH + oldPoint.x] = nullptr;
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
                    std::abs(creature->getPoint().x - getPoint().x)
                        + std::abs(creature->getPoint().y - getPoint().y) 
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
        return;
    }
    setState(Moving);
    Creatures::Direction direction = bestDirection(closetMonkey->getPoint(), field);
    move(direction, field);
}

void Monkey::Ai(Field<Config::HEIGHT, Config::WIDTH>& field)
{ 
    //how the monkey deals with his neighbors
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
            if(neighbor == nullptr)
            {
                continue;
            }
            else if(neighbor->getType() == Creatures::Dog)
            { 
                Creatures::Direction bestDir { fleeDirection(neighbor->getPoint(), field) };
                move(bestDir, field);
                setState(Moving);
                return;
            }
            else if(neighbor->getType() == Creatures::Human)
            { 
                if(rand() % 2)
                { 
                    Creatures::Direction bestDir { fleeDirection(neighbor->getPoint(), field) };
                    move(bestDir, field);
                    setState(Moving);
                    setMoved(true);
                    return;
                }
                else
                { 
                    attack(*neighbor);
                    return;
                }
            }
            else if(neighbor->getType() == Creatures::House)
            {
                attack(*neighbor);
                return;
            }

            else if(neighbor->getType() == Creatures::Crops)
            {
                attack(*neighbor);
                addCorn();
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

            int distance { 
                std::abs(target->getPoint().x - getPoint().x) + 
                    std::abs(target->getPoint().y - getPoint().y)
            };

            if(distance < minDistance)
            { 
                minDistance = distance;
                closetTarget = target;
                target = nullptr;
            }
        }
    }

    if(!closetTarget)
    { 
        setState(Idle);
        move(Stop, field);
        return;
    }

    Creatures::Direction bestDir = bestDirection(closetTarget->getPoint(), field);
    move(bestDir, field);
}

#endif
