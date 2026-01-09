#include "Constants.hpp"
#include "Creatures.hpp"
#include "Field.hpp"
#include "Brain.hpp"
#include "Crops.hpp"
#include "Dog.hpp"
#include "House.hpp"
#include "Human.hpp"
#include "Monkey.hpp"


#include <iostream>
#include <chrono>
#include <thread>
#include <ctime>
#include <cstdlib>

int main() 
{ 
    srand(time(NULL));
    Field<Config::HEIGHT, Config::WIDTH> ground {};
    std::chrono::milliseconds dur{ Config::SPEED };

    while(ground.getMonkeyCount()) 
    { 
        std::cout << "\033[2J" << '\n';
        std::cout << ground;
        ground.updateGrid();
        std::this_thread::sleep_for(dur);
    }
    return 0;
}
