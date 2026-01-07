#ifndef HOUSE_HPP
#define HOUSE_HPP

#include "Creatures.hpp"
#include "Constants.hpp"
// A house has massive amounts of health but no damage
template<int H, int W>
class Field;

class House:public Creatures
{ 
    public:
        House(int x, int y, int id)
            :Creatures(Point{x, y}, Creatures::House, id, 'H', 550, 0)
            {}
        void Ai(Field<Config::HEIGHT, Config::WIDTH>& field) override;
};

#endif
