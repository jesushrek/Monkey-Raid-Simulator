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
        Field() 
        { 
            //for crops
            for(int i = 0; i < 3; ++i)
            { 
                for(int j = 0; j < WIDTH; ++j)
                { 
                    if(rand() % 2)
                    { 
                        m_grid[i*WIDTH + j] = new Crops(j, i, m_cropCount);
                        ++m_cropCount;
                    }
                }
            }

            //for Monkeys
            for(int i = 0; i < HEIGHT; ++i)
            { 
                for(int j = 0; j < WIDTH; ++j)
                { 
                    if((rand() % 100) <= 10)
                    { 
                        if(m_grid[i*WIDTH + j] == nullptr)
                        { 
                            m_grid[i*WIDTH + j] = new Monkey(j, i, m_monkeyCount);
                            ++m_monkeyCount;
                        }
                    }
                }
            }

            //for Villagers
            for(int i = 0; i < HEIGHT; ++i)
            {
                for(int j = 0; j < WIDTH; ++j)
                { 
                    if((rand() % 100) <= 20)
                    { 
                        if(m_grid[i*WIDTH + j] == nullptr)
                        { 
                            m_grid[i*WIDTH + j] = new Human(j, i, m_humanCount);
                            ++m_humanCount;
                        }
                    }
                }
            }

            //for village dogs
            for(int i = 0; i < HEIGHT; ++i)
            { 
                for(int j = 0; j < WIDTH; ++j)
                { 
                    if((rand() % 100) <= 10)
                    { 
                        if(m_grid[i*WIDTH + j] == nullptr)
                        { 
                            m_grid[i*WIDTH + j] = new Dog(j, i, m_dogCount);
                        }
                        ++m_dogCount;
                    }
                }
            }

            int toBePlaced = m_humanCount / 3;
            m_houseCount = 0;
            bool onObject = false;

            for(int i = 0; i < toBePlaced; ++i)
            { 
                bool placed = false;

                while(!placed)
                { 
                    int x = rand() % WIDTH;
                    int y = rand() % HEIGHT;

                    int idx = y * HEIGHT + x;

                    if(m_grid[idx] == nullptr)
                    { 
                        m_grid[idx] = new House(x, y, m_houseCount);
                        ++m_houseCount;
                        placed = true;
                    }
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
