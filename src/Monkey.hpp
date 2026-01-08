#ifndef MONKEY_HPP
#define MONKEY_HPP

#include "Creatures.hpp"
#include "Constants.hpp"
// A monkey has damage of 2 and health of 10 by default

template <int H, int W>
class Field;

class Monkey:public Creatures
{ 
    private:
        int m_cornCount{};
    public:
        Monkey(int x, int y, int id)
            :Creatures { Point{x, y}, Creatures::Monkey, id, 'm', 120, 2 }, m_cornCount{0}
        {}
        virtual void Ai(Field<Config::HEIGHT, Config::WIDTH>& field) override;
        int getCorn() const { return m_cornCount; }
        void addCorn() { ++m_cornCount; } 
};

#endif
