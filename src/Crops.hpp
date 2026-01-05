#ifndef CROPS_HPP
#define CROPS_HPP
#include "Creatures.hpp"

class Crops:public Creatures 
{ 
    public:
        Crops(int id)
            :Creatures (Creatures::Crops, id, 'c', 1, 0)
        {}
};
#endif
