#ifndef CROPS_HPP
#define CROPS_HPP
#include "Creatures.hpp"
#include "Constants.hpp"

template<int H, int W>
class Field;

class Crops:public Creatures 
{ 
   public:
        Crops(int x, int y, int id)
            :Creatures (Point{ x, y }, Creatures::Crops, id, 'c', 8, 0)
            {}
        virtual void Ai(Field<Config::HEIGHT, Config::WIDTH>& field) override;
};
#endif
