#ifndef ITEM_H
#define ITEM_H

#include <string>

namespace ItemTypes {
    static const int MAINHAND = 0;
    static const int OFFHAND = 1;
    static const int RANGED = 2;
    static const int HEAD = 3;
    static const int NECK = 4;
    static const int SHOULDERS = 5;
    static const int BACK = 6;
    static const int CHEST = 7;
    static const int WRIST = 8;
    static const int GLOVES = 9;
    static const int BELT = 10;
    static const int LEGS = 11;
    static const int BOOTS = 12;
    static const int RING = 13;
    static const int TRINKET = 14;
    static const int CASTER_OFFHAND = 15;
    static const int RELIC = 16;
}

class Item {
public:
    virtual ~Item() {}
    Item(std::string _name): name(_name) {}
    virtual int get_item_type(void) const = 0;

    std::string get_name(void) const { return name; }

protected:
    std::string name;

private:
};

#endif // ITEM_H
