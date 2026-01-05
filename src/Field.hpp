#ifndef FIELD_HPP
#define FIELD_HPP

#include "Creatures.hpp"
#include "Crops.hpp"
#include "Monkey.hpp"
#include "House.hpp"
#include "Human.hpp"
#include "Dog.hpp"

#include <ostream>
#include <cstdlib>
#include <array>

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
        Field() 
        { 
            //for crops
            for(int i = 0; i < 3; ++i)
            { 
                for(int j = 0; j < WIDTH; ++j)
                { 
                    if(rand() % 2)
                    { 
                        m_grid[i*WIDTH + j] = new Crops(m_cropCount);
                        ++m_cropCount;
                    }
                }
            }

            //for Monkeys
            for(auto& object : m_grid)
            { 
                if((rand() % 100) <= 15)
                { 
                    if(object == nullptr)
                    { 
                        object = new Monkey(m_monkeyCount);
                        ++m_monkeyCount;
                    }
                }
            }

            //for Villagers
            for(auto& object : m_grid)
            { 
                if((rand() % 100) <= 20)
                { 
                    if(object == nullptr)
                    { 
                        object = new Human(m_humanCount);
                        ++m_humanCount;
                    }
                }
            }

            //for village dogs
            for(auto& object : m_grid)
            { 
                if((rand() % 100) <= 10)
                { 
                    object = new Dog(m_dogCount);
                    ++m_dogCount;
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

            out << "House/s: " << f.m_houseCount
                << "Monkey/s: " << f.m_monkeyCount
                << "Dog/s: " << f.m_dogCount
                << "Villager/s: " << f.m_humanCount 
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
