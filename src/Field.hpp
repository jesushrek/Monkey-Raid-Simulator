#ifndef FIELD_HPP
#define FIELD_HPP
#include "Creatures.hpp"
#include "Crops.hpp"
#include "Monkey.hpp"
#include "Human.hpp"
#include "Dog.hpp"
#include "House.hpp"

#include <ostream>
#include <cstdlib>
#include <array>

class Crops;
class Monkey;
class Human;
class Dog;
class Creatures;

template<int HEIGHT, int WIDTH>
class Field
{ 
    private:
        std::array<Creatures*, HEIGHT*WIDTH> m_grid{};
        int m_cropCount{};
        int m_monkeyCount{};
        int m_humanCount{};
        int m_houseCount{};
        int m_dogCount{};
    public:
        //all the getters
        auto& getGrid() { return m_grid; } 
        int getMonkeyCount() { return m_monkeyCount; }
        Field() 
        { 
            //crops
            int fieldSize{ WIDTH / 3 };
            int dx{ (WIDTH - fieldSize) / 2 };
            int dy{ (HEIGHT - fieldSize) / 2};

            for(int i = 0; i < fieldSize; ++i)
            { 
                for(int j = 0; j < fieldSize; ++j)
                { 
                    int x {(i+dx)};
                    int y {(j+dy)};
                    m_grid[y * WIDTH + x] = new Crops{x, y, x+i};
                    ++m_cropCount;
                }
            }

            summon(Creatures::Monkey, 20);
            summon(Creatures::Human, 15);
            summon(Creatures::House, 20);
            summon(Creatures::Dog, 3);
        }

        void summon(Creatures::Type t, int count)
        { 
            int entityToSummon = count;
            while(entityToSummon)
            { 
                int x {rand() % WIDTH};
                int y {rand() % HEIGHT};

                int idx { y * WIDTH + x };

                if(m_grid[idx]) continue;

                if(t == Creatures::Monkey) { 
                    m_grid[idx] = new Monkey{ x, y, x+y};
                    ++m_monkeyCount;
                }
                if(t == Creatures::Human) { 
                    m_grid[idx] = new Human{ x, y, x+y};
                    ++m_humanCount;
                }
                if(t == Creatures::Dog) { 
                    m_grid[idx] = new Dog{ x, y, x+y};
                    ++m_dogCount;
                }
                if(t == Creatures::House) { 
                    m_grid[idx] = new House{ x, y, x+y};
                    ++m_houseCount;
                }

                --entityToSummon;
            }
        }

        void updateGrid()
        { 
            //set every pointer to false.
            for(auto& creature : m_grid)
            { 
                if(creature == nullptr) continue;
                creature->setMoved(false);
            }

            for(auto& creature : m_grid)
            {
                if(creature != nullptr && !creature->getMoved())
                    creature->Ai(*this);
            }

            for(auto& creature : m_grid)
            { 
                if(creature == nullptr)
                    continue;

                if(!creature->getStatus())
                { 
                    //decrement the counters for dead guys
                    switch(creature->getType())
                    { 
                        case Creatures::Monkey:
                            --m_monkeyCount;
                            break;
                        case Creatures::Crops:
                            --m_cropCount;
                            break;
                        case Creatures::Dog:
                            --m_dogCount;
                            break;
                        case Creatures::Human:
                            --m_humanCount;
                            break;
                        case Creatures::House:
                            --m_houseCount;
                            break;
                        default:
                            break;
                    };

                    delete creature;
                    creature = nullptr;
                }
            }
        }

        friend std::ostream& operator<<(std::ostream& out, const Field<HEIGHT, WIDTH>& f)
        { 
            for(int i = 0; i < HEIGHT; ++i)
            { 
                out << '|';
                for(int j = 0; j < WIDTH; ++j)
                { 
                    int idx = i*WIDTH + j;
                    if(f.m_grid[idx] != nullptr)
                        out << f.m_grid[idx]->getSymbol();
                    else
                        out << ' ';
                }
                out << "|\n";
            }

            out << "\nHouse(s): " << f.m_houseCount
                << "\nMonkey(s): " << f.m_monkeyCount
                << "\nDog(s): " << f.m_dogCount
                << "\nVillager(s): " << f.m_humanCount 
                << '\n';
            return out;
        }

        Field operator=(const Field& f) = delete;
        Field(const Field& other) = delete;

        ~Field() 
        { 
            for(auto object : m_grid)
            { 
                if(object != nullptr)
                    delete object;
            }
        }
};

#endif
