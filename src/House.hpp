#ifndef HOUSE_HPP
#define HOUSE_HPP

#include "Creatures.hpp"
// A house has massive amounts of health but no damage
class House:public Creatures
{ 
    public:
        House(int id)
            :Creatures(Creatures::House, id, 'H', 50, 0)
        {}
};

#endif
