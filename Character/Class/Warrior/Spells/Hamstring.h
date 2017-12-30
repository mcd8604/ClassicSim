#ifndef HAMSTRING_H
#define HAMSTRING_H

#include "Spell.h"

class Hamstring: public Spell {
public:
    Hamstring(Engine* engine, Character* pchar, CombatRoll* roll) :
        Spell("Hamstring", engine, pchar, roll, 0, 10) {}

    int spell_effect(const int) const override;
protected:
private:
};

#endif // HAMSTRING_H