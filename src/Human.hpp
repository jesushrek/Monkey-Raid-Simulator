#ifndef HUMAN_HPP
#define HUMAN_HPP

#include "Creatures.hpp"
// A human has damage of 1 and health of 15 by default and symbol V as in Village man
class Human:public Creatures
{ 
    public:
        Human(int id)
            :Creatures { Creatures::Human, id, 'V', 15, 1 }
        {}
};

#endif
