#ifndef HUMAN_HPP
#define HUMAN_HPP

#include "Creatures.hpp"
#include "Constants.hpp"
// A human has damage of 1 and health of 15 by default and symbol V as in Village man
template<int H, int W>
class Field;

class Human:public Creatures
{ 
    public:
        Human(int x, int y, int id)
            :Creatures { Point {x, y}, Creatures::Human, id, 'V', 250, 1 }
        {}
        virtual void Ai(Field<Config::HEIGHT, Config::WIDTH>& field) override;
};

#endif
