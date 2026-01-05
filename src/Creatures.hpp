#ifndef CREATURES_HPP
#define CREATURES_HPP

class Creatures
{ 
    public:
        enum Type 
        { 
            Crops = 0,
            Human,
            Monkey,
            House,
            Dog,
            MaxTypes
        };

        Type getType() const { return m_type; }
        int getHealth() const  { return m_health; }
        int getDamage() const { return m_damage; }
        int getId() const { return m_id; }
        char getSymbol() const { return m_symbol; }
        bool getStatus() const { return m_isAlive; }
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

    protected:
        Creatures(Type type, int id, char symbol, int health, int damage)
            :m_type{type}, m_id{id}, m_symbol{ symbol }, m_health{health}, m_damage{damage}
        {}

    private:
        Type m_type{};
        int m_id{};
        char m_symbol{};
        int m_health{};
        int m_damage{};
        bool m_isAlive{true};
};

#endif
