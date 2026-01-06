#ifndef CREATURES_HPP
#define CREATURES_HPP
#include "Constants.hpp"
#include <cmath>

template <int HEIGHT, int WIDTH>
class Field;

class Creatures
{ 
    public:
        enum Direction
        { 
            Up = 0,
            Down, 
            Left, 
            Right,
            MaxDirections,
        };

        enum State 
        { 
            Moving = 0,
            Idle,
        };

        enum Type 
        { 
            Crops = 0,
            Human,
            Monkey,
            House,
            Dog,
            MaxTypes
        };

        struct Point
        { 
            int x{};
            int y{};
        };

        // all the getters
        Point getPoint() const { return m_point; }
        Type getType() const { return m_type; }
        int getHealth() const  { return m_health; }
        int getDamage() const { return m_damage; }
        int getId() const { return m_id; }
        char getSymbol() const { return m_symbol; }
        bool getStatus() const { return m_isAlive; }
        Direction getDirection() const { return m_direction; }
        State getState() const { return m_state; }
        virtual ~Creatures() {};
        void takeDamage(int amount) 
        { 
            m_health -= amount;
            if(m_health <= 0)
            { 
                m_health = 0;
                m_isAlive = false;
            }
        }

        void attack(Creatures& target)
        { 
            if(target.getStatus())
            { 
                target.takeDamage(m_damage);
            }
        }

        virtual void Ai(Field<Config::HEIGHT, Config::WIDTH>& field) = 0;
        Direction bestDirection(Point to, Field<Config::HEIGHT, Config::WIDTH>& field);
        //all the setters
        void setPoint(Point point) { m_point = point; }
        void setDirection(Direction direction) { m_direction = direction; }
        void setState(State state) { m_state = state; } 
        void setHealth(int health) { m_health = health; }
        void setDamage(int damage) { m_damage = damage; }
        void setStatus(bool isAlive) { m_isAlive = isAlive; }

    protected:
        Creatures(Point point, Type type, int id, char symbol, int health, int damage)
            :m_point{point} ,m_type{type}, m_id{id}, m_symbol{ symbol }, m_health{health}, m_damage{damage}
        {}

    private:
        Point m_point;
        Type m_type{};
        int m_id{};
        char m_symbol{};
        int m_health{};
        int m_damage{};
        Direction m_direction{};
        State m_state{Idle};
        bool m_isAlive{true};
};

#endif
