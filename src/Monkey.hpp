#ifndef MONKEY_HPP
#define MONKEY_HPP

#include "Creatures.hpp"
// A monkey has damage of 2 and health of 5 by default
class Monkey:public Creatures
{ 
    private:
        int m_cornCount{};
    public:
        Monkey(int id)
            :Creatures { Creatures::Monkey, id, 'm', 5, 2 }, m_cornCount{0}
        {}
        int getCorn() const { return m_cornCount; }
};

#endif
